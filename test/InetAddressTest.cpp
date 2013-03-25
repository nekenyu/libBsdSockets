#include <cppunit/extensions/HelperMacros.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdexcept>
#include <system_error>
#include <sstream>
#include <memory>

#include "LowLevelAddress.h"
#include "InetAddress.h"
using namespace BsdSockets;

class InetAddressTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(InetAddressTest);
  CPPUNIT_TEST_EXCEPTION(blankBoth, std::system_error);
  CPPUNIT_TEST_EXCEPTION(badServiceNumber, std::system_error);
  CPPUNIT_TEST_EXCEPTION(badServiceName, std::system_error);
  CPPUNIT_TEST_EXCEPTION(badHostname, std::system_error);
  CPPUNIT_TEST(hostnameLoopback4);
  CPPUNIT_TEST(hostnameLoopback6);
  CPPUNIT_TEST(serviceNameSmtp);
  CPPUNIT_TEST(serviceNumber);
  CPPUNIT_TEST(hostnameLoopback4Single);
  CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp() {
  }

  void tearDown() {
  }

  void blankBoth() {
    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, "");
  }

  void badServiceNumber() {
    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, "9999999999999");
  }

  void badServiceName() {
    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, "(*&)(*&)(*&)(*&)");
  }

  void badHostname() {
    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, "", "foo...");
  }

  void hostnameLoopback4() {
    const std::string requested = "127.0.0.1";
    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, "", requested);
    CPPUNIT_ASSERT(SocketDomain::INET4 == addr->getSocketDomain());
    CPPUNIT_ASSERT_EQUAL(requested, addr->getRequestedAddress());
    CPPUNIT_ASSERT_EQUAL(requested, addr->getActualAddress());
    CPPUNIT_ASSERT(SocketType::STREAM == addr->getSocketType());

    const struct sockaddr_in& sin = (sockaddr_in&) addr->getLowLevelAddress().getSockAddr();
    const u_long s_addr = sin.sin_addr.s_addr;
    const u_long s_addr_host = ntohl(s_addr);
    const u_long expected = (127 << 24) | 1;
    CPPUNIT_ASSERT_EQUAL(expected, s_addr_host);
  }

  void hostnameLoopback6() {
    const std::string requested = "::1";
    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, "", requested);
    CPPUNIT_ASSERT(SocketDomain::INET6 == addr->getSocketDomain());
    CPPUNIT_ASSERT(requested == addr->getRequestedAddress());
    CPPUNIT_ASSERT("::" == addr->getActualAddress());
    CPPUNIT_ASSERT(SocketType::STREAM == addr->getSocketType());
  }

  void serviceNameSmtp() {
    const std::string requested = "smtp";
    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, requested);
    CPPUNIT_ASSERT_EQUAL(requested, addr->getServiceName());
    CPPUNIT_ASSERT_EQUAL(25u, addr->getPort());
  }

  void serviceNumber() {
    const unsigned int port = 12345;
    std::ostringstream str;
    str << port;
    const std::string requested = str.str();

    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, requested);
    CPPUNIT_ASSERT_EQUAL(port, addr->getPort());
  }

  void hostnameLoopback4Single() {
    const std::string requested = "127.0.0.1";
    std::vector<InetAddress::Ptr> found;
    InetAddress::create(SocketType::STREAM, "", requested, found);
    CPPUNIT_ASSERT_EQUAL(1, (int) found.size());
    CPPUNIT_ASSERT_EQUAL(requested, found.at(0)->getActualAddress());
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(InetAddressTest);
