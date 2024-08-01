/*

Copyright (c) 2013, Komodo Does Inc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Komodo Does Inc nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#ifndef SOCKET_H
#include "Socket.h"
#endif // SOCKET_H

#include <functional>

namespace BsdSockets {

  /*
   * Forward Declarations
   */
  class Address;

  /** \class ServerSocket
   *
   * \brief Server Socket providing ability to bind, listen and accept connections
   *
   * Datagram sockets may send/receive with the ServerSocket directly.
   * Other Sockets use the accept() returned Sockets.
   */
  class ServerSocket : public Socket {
  public:
    typedef std::shared_ptr<ServerSocket> Ptr;

    /** Create a ServerSocket to theAddress.  This still needs bind() and listen().
     *
     * @param theAddress to open, bind and listen the ServerSocket on
     * @return the socket
     *
     * @see %listen() parameter backlog for more details
     */
    static ServerSocket::Ptr create(std::shared_ptr<Address> theAddress);

    /** Create and open a ServerSocket to theAddress.
     *
     * @param theAddress to open, bind and listen the ServerSocket on
     * @param backlog length before events error
     * @param reuse true to allow reuse of address and port, or false for default for timeout before reuse
     * @return the socket
     *
     * @see %listen() parameter backlog for more details
     */
    static ServerSocket::Ptr open(std::shared_ptr<Address> theAddress, int backlog = 128, bool reuse = false);

    /** Closes socket.  Virtual destructor to support derived classes. */
    virtual ~ServerSocket();

  protected:
    /** Create ServerSocket to listen to the given address.
     *
     * @param theAddress to bind and listen to
     */
    ServerSocket(std::shared_ptr<Address> theAddress);

  private:
    ServerSocket(const Socket& rhs) = delete;
    ServerSocket(Socket&&) = delete;
    ServerSocket& operator=(const Socket& rhs) = delete;
    ServerSocket& operator=(Socket&& rhs) = delete;

  public:
    /** Bind this Socket to the address */
    void bind() const;

    /** Listen to the Socket with the given backlog.
     *
     * @param backlog maximum length for the queue of pending connections.
     *
     * @see %listen() parameter backlog for more details
     */
    void listen(int backlog = 128) const;

    /** Accept a connection on the Server Socket.
     *
     * @return the Socket accepted
     */
    Socket::Ptr accept() const;

    /** Accept a connection on the Server Socket and pass it as the argument to
     * the function-wrapper toCall.
     *
     * @param toCall function to call with new socket
     *
     * @return thread created
     */
    std::thread acceptInNewThread(std::function<void (Socket::Ptr)> toCall);
  };

} // namespace BsdSockets

#endif // SERVER_SOCKET_H
