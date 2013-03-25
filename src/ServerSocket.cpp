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
#include <system_error>
#include <memory>

#include "ServerSocket.h"
#include "Address.h"
#include "LowLevelAddress.h"

namespace BsdSockets {

  ServerSocket::Ptr ServerSocket::create(Address::Ptr theAddress) {
    return ServerSocket::Ptr (new ServerSocket(theAddress));
  }

  ServerSocket::Ptr ServerSocket::open(Address::Ptr theAddress, int backlog) {
    ServerSocket::Ptr socket = create(theAddress);
    socket->bind();
    socket->listen(backlog);
    return socket;
  }

  ServerSocket::~ServerSocket() {
  }

  ServerSocket::ServerSocket(Address::Ptr theAddress)
    : Socket(theAddress)
  {
  }

  void ServerSocket::bind() const {
    const LowLevelAddress& lowLevelAddress = getAddress()->getLowLevelAddress();

    if(-1 == ::bind(getLowLevelSocket(), &lowLevelAddress.getSockAddr(), lowLevelAddress.getSockAddrLen())) {
      throw std::system_error(errno, std::system_category());
    }
  }

  void ServerSocket::listen(int backlog) const {
    if(-1 == ::listen(getLowLevelSocket(), backlog)) {
      throw std::system_error(errno, std::system_category());
    }
  }

  Socket::Ptr ServerSocket::accept() const {
    LowLevelAddress::Ptr tmp = getAddress()->makeTempLowLevelAddress();
    socklen_t len;

    const int socketCreated = ::accept(getLowLevelSocket(), &tmp->getSockAddr(), &len);
    if(len > tmp->getSockAddrLen()) {
      throw std::runtime_error("accept over-write the length of the sockaddr for the ServerSocket Address");
    }

    if(-1 == socketCreated) {
      throw std::system_error(errno, std::system_category());
    }
    if(AF_UNSPEC == tmp->getSockAddr().sa_family) {
      tmp->getSockAddr().sa_family = getAddress()->getLowLevelAddress().getSockAddr().sa_family;
    }
    
    Address::Ptr acceptedAddress = Address::Ptr(getAddress()->create(tmp));
    return Socket::create(acceptedAddress, socketCreated);
  }

} // namespace BsdSockets
