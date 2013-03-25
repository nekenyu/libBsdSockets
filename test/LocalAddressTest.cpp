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
#include <sys/un.h>
#include <stdexcept>
#include <memory>

#include "LowLevelAddress.h"
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
    LocalAddress::Ptr addr = LocalAddress::create("");
  }

  void goodPath() {
    const std::string requested = "p";
    LocalAddress::Ptr addr = LocalAddress::create(requested);
    CPPUNIT_ASSERT(SocketDomain::LOCAL == addr->getSocketDomain());
    CPPUNIT_ASSERT_EQUAL(requested, addr->getPath());

    // This is ugly, but it wouldn't let me reinterpret cast without pointers
    // and I hate c-style casts...
    const struct sockaddr_un& ref = *reinterpret_cast<const struct sockaddr_un*>(&addr->getLowLevelAddress().getSockAddr());
    CPPUNIT_ASSERT(requested == ref.sun_path);
    CPPUNIT_ASSERT(SocketType::STREAM == addr->getSocketType());
  }

  void pathTooLong() {
    LocalAddress::Ptr addr = LocalAddress::create(std::string().assign(256,'z'));
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(LocalAddressTest);
