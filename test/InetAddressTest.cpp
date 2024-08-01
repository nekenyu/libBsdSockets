/*

Copyright (c) 2013, Komodo Does Inc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Komodo Does Inc nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

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
    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, requested, "127.0.0.1");
    CPPUNIT_ASSERT_EQUAL(requested, addr->getServiceName());
    CPPUNIT_ASSERT_EQUAL(25u, addr->getPort());
  }

  void serviceNumber() {
    const unsigned int port = 12345;
    std::ostringstream str;
    str << port;
    const std::string requested = str.str();

    InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, requested, "127.0.0.1");
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
