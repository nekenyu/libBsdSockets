/*

Copyright (c) 2013, Komodo Does Inc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Komodo Does Inc nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <memory>

#include <stdexcept>
#include <string>

#include "LocalAddress.h"

#include "LowLevelAddress.h"

namespace BsdSockets {
  typedef LowLevelAddressType<struct sockaddr_un> LocalLowLevelAddress;

  /** \brief The private implementation of LocalAddress.
   *
   * This provides the LowLevelAddress.
   */
  class LocalAddressPimpl : public LocalLowLevelAddress {
  public:
    /** Create by encoding thePath
     *
     * @param thePath to encode
     * @param allowBlank true if blank is allowed, which is really only from accept()
     */
    LocalAddressPimpl(const std::string& thePath, bool allowBlank)
    {
      if(!allowBlank && 0 == thePath.size()) {
	throw std::invalid_argument("thePath cannot be blank");
      }

      if(thePath.size() >= sizeof(lowLevelAddress.sun_path)) {
	throw std::invalid_argument("thePath is too long for this system");
      }

      lowLevelAddress.sun_family = AF_LOCAL;
      strcpy(lowLevelAddress.sun_path, thePath.c_str());
    }

  private:
    LocalAddressPimpl() = delete;
    LocalAddressPimpl(const LocalAddressPimpl& rhs) = delete;
    LocalAddressPimpl(LocalAddressPimpl&& rhs) = delete;
    LocalAddressPimpl& operator=(const LocalAddressPimpl& rhs) = delete;
    LocalAddressPimpl& operator=(LocalAddressPimpl&& rhs) = delete;
  };

  /*
   * LocalAddress Implementation
   */

  LocalAddress::Ptr LocalAddress::create(const std::string& thePath) {
    return LocalAddress::Ptr(new LocalAddress(thePath, false));
  }

  LocalAddress::~LocalAddress() {
  }

  LocalAddress::LocalAddress(const std::string& thePath, bool allowBlank)
    : Address(SocketDomain::LOCAL, SocketType::STREAM, 0),
      path(thePath), pimpl(new LocalAddressPimpl(thePath, allowBlank))
  {
    if(nullptr == pimpl) {
      throw new std::invalid_argument("Invalid path");
    }
  }

  const std::string& LocalAddress::getPath() const {
    return path;
  }

  LowLevelAddress::Ptr LocalAddress::makeTempLowLevelAddress() const {
    return LowLevelAddress::Ptr(new LocalLowLevelAddress());
  }

  Address::Ptr LocalAddress::create(std::shared_ptr<LowLevelAddress> lowLevelAddress) const {
    if(AF_LOCAL != lowLevelAddress->getSockAddr().sa_family) {
      throw std::invalid_argument("LowLevelAddress is of wrong type for LocalAddress to create");
    }

    const struct sockaddr_un& ref = *reinterpret_cast<const struct sockaddr_un*>(&lowLevelAddress->getSockAddr());
    return Address::Ptr(new LocalAddress(ref.sun_path, true));
  }

  LowLevelAddress& LocalAddress::getLowLevelAddress() const {
    return *pimpl;
  }

} // namespace BsdSockets


