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
#include <sys/select.h>
#include <unistd.h>
#include <poll.h>

#include <system_error>
#include <memory>

#include "Address.h"
#include "AddressInfoConversions.h"
#include "Socket.h"

namespace BsdSockets {

  Socket::~Socket() {
    if(-1 == lowLevelSocket) {
      return;
    }

    // Close it, ignoring the consequences since there is nothing that can be done
    close(lowLevelSocket);
  }

  Socket::Ptr Socket::create(Address::Ptr theAddress, int existingLowLevelSocket) {
    return Socket::Ptr(new Socket(theAddress, existingLowLevelSocket));
  }

  Socket::Socket(Address::Ptr theAddress)
    : lowLevelSocket(::socket(
		      socketDomainToLowLevel(theAddress->getSocketDomain()),
		      socketTypeToLowLevel(theAddress->getSocketType()),
		      theAddress->getProtocol()
		      )),
      address(theAddress)
  {
  }

  Socket::Socket(Address::Ptr theAddress, int existingLowLevelSocket)
    : lowLevelSocket(existingLowLevelSocket),
      address(theAddress)
  {
    if(-1 == lowLevelSocket) {
      throw std::system_error(errno, std::system_category());
    }
  }

  SelectResult Socket::select(int timeout_ms, bool checkRead, bool checkWrite, bool checkError) {
    struct timeval timeout;
    if(0 != timeout_ms) {
      timeout.tv_sec = timeout_ms / 1000;
      timeout.tv_usec = 1000 * (timeout_ms % 1000);
    }
  
    fd_set read;  FD_ZERO(&read);  if(checkRead) { FD_SET(lowLevelSocket, &read); }
    fd_set write; FD_ZERO(&write); if(checkWrite) { FD_SET(lowLevelSocket, &write); }
    fd_set error; FD_ZERO(&error); if(checkError) { FD_SET(lowLevelSocket, &error); }
    const int result = ::select(lowLevelSocket + 1, &read, &write, &error, 0 == timeout_ms ? nullptr : &timeout);
    if(-1 == result) {
      throw std::system_error(errno, std::system_category());
    }

    const bool readyToRead = FD_ISSET(lowLevelSocket, &read);
    const bool readyToWrite = FD_ISSET(lowLevelSocket, &write);
    const bool readyWithError = FD_ISSET(lowLevelSocket, &error);

    return SelectResult(checkRead, checkWrite, checkError, readyToRead, readyToWrite, readyWithError);
  }

  short Socket::poll(int timeout_ms, short eventsToLookFor) {
     struct pollfd fds[1];
  
     fds[0].fd = lowLevelSocket;
     fds[0].events = eventsToLookFor;

    if(-1 == ::poll(fds, 1, timeout_ms)) {
      throw std::system_error(errno, std::system_category());
    }

    return fds[0].revents;
  }

  ssize_t Socket::send(const void* buffer, size_t length, int flags) const {
    const ssize_t result = ::send(lowLevelSocket, buffer, length, flags);
    if(-1 != result) {
      return result;
    }

    throw std::system_error(errno, std::system_category());
  }

  ssize_t Socket::receive(void* buffer, size_t length, int flags) const {
    const ssize_t result = ::recv(lowLevelSocket, buffer, length, flags);
    if(-1 != result) {
      return result;
    }

    throw std::system_error(errno, std::system_category());
  }

  ssize_t Socket::blockingReceive(void* buffer, size_t length) const {
    const ssize_t result = ::read(lowLevelSocket, buffer, length);
    if(-1 != result) {
      return result;
    }

    throw std::system_error(errno, std::system_category());
  }

  void Socket::getSocketOption(int level, int optionName, void* optionValue, socklen_t* optionLength) const {
    if(-1 == ::getsockopt(lowLevelSocket, level, optionName, optionValue, optionLength)) {
      throw std::system_error(errno, std::system_category());
    }
  }

  void Socket::setSocketOption(int level, int optionName, const void* optionValue, socklen_t optionLength) const {
    if(-1 == ::setsockopt(lowLevelSocket, level, optionName, optionValue, optionLength)) {
      throw std::system_error(errno, std::system_category());
    }
  }

  std::shared_ptr<Address> Socket::getAddress() const {
    return address;
  }

  int Socket::getLowLevelSocket() const {
    return lowLevelSocket;
  }

} // namespace BsdSockets
