#include <cppunit/extensions/HelperMacros.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <stdexcept>

#include "RawAddress.h"
#include "LocalAddress.h"
using namespace BsdSockets;

/** Test LocalAddress class.
 *
 * The main thing to test is the path string.
 */
class LocalAddressTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(LocalAddressTest);
  CPPUNIT_TEST_EXCEPTION(blankPath, std::invalid_argument);
  CPPUNIT_TEST(goodPath);
  CPPUNIT_TEST_EXCEPTION(pathTooLong, std::invalid_argument);
  CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp() {
  }

  void tearDown() {
  }

  void blankPath() {
    const LocalAddress addr("");
  }

  void goodPath() {
    const std::string requested = "p";
    const LocalAddress addr(requested);
    CPPUNIT_ASSERT(SocketDomain::LOCAL == addr.getSocketDomain());
    CPPUNIT_ASSERT_EQUAL(requested, addr.getPath());

    // This is ugly, but it wouldn't let me reinterpret cast without pointers
    // and I hate c-style casts...
    const struct sockaddr_un& ref = *reinterpret_cast<const struct sockaddr_un*>(&addr.getImpl().getSockAddr());
    CPPUNIT_ASSERT(requested == ref.sun_path);
    CPPUNIT_ASSERT(SocketType::STREAM == addr.getSocketType());
  }

  void pathTooLong() {
    const LocalAddress addr(std::string().assign(256,'z'));
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(LocalAddressTest);
