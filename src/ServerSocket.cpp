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
#include <functional>
#include <thread>

#include "ServerSocket.h"
#include "Address.h"
#include "LowLevelAddress.h"

namespace {
  using namespace BsdSockets;

  /** Helper method and compound result for Accepting a socket
   */
  class AcceptedSocketData {
  public:
    /** Create with data
     *
     * @param theLowLevelSocket of the accepted socket
     * @param theAddress of the accepted socket
     */
    AcceptedSocketData(int theLowLevelSocket, Address::Ptr theAddress)
      :lowLevelSocket(theLowLevelSocket), address(theAddress)
    {
    }

  public:
    /** low-level socket id */
    const int lowLevelSocket;
    
    /** Address connected to */
    Address::Ptr address;
  };

  /** Do low level socket accept and prepare Address connected to
   *
   * @param serverLowLevelSocket low-level socket id for server to accept from
   * @param serverAddress address of the server socket to build the address of the accepted socket
   *
   * @return data for the accepted socket
   */
   AcceptedSocketData lowLevelAccept(int serverLowLevelSocket, Address::Ptr serverAddress) {
     // Accept the socket
      LowLevelAddress::Ptr lowLevelAddress = serverAddress->makeTempLowLevelAddress();
      socklen_t len;
      const int lowLevelSocket = ::accept(serverLowLevelSocket, &lowLevelAddress->getSockAddr(), &len);

      // Verify results
      if(len > lowLevelAddress->getSockAddrLen()) {
	throw std::runtime_error("accept over-write the length of the sockaddr for the ServerSocket Address");
      }

      if(-1 == lowLevelSocket) {
	throw std::system_error(errno, std::system_category());
      }

      // Assume the server socket's type if unspecified
      // TODO: I'd like to do better, but there are no guarantees if we are getting a different 
      // address tyle, like unspecified accepting from a local socket
      if(AF_UNSPEC == lowLevelAddress->getSockAddr().sa_family) {
	lowLevelAddress->getSockAddr().sa_family = serverAddress->getLowLevelAddress().getSockAddr().sa_family;
      }

      // Build the results
      return AcceptedSocketData(lowLevelSocket, serverAddress->create(lowLevelAddress));
    }

} // namespace

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
    AcceptedSocketData data = lowLevelAccept(getLowLevelSocket(), getAddress());
    return Socket::create(data.address, data.lowLevelSocket);
  }

  std::thread ServerSocket::acceptInNewThread(std::function<void (Socket::Ptr)> toCall) {
    // Note: We rely on the thread-safety of shared_ptr for the address....
    // This shouldn't be a big deal since it will clear from data in this frame
    // and thus this thread
    AcceptedSocketData data = lowLevelAccept(getLowLevelSocket(), getAddress());
    return std::thread ([=]() {
	toCall(Socket::create(data.address, data.lowLevelSocket));
      });
  }

} // namespace BsdSockets
