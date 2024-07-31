/*

Copyright (c) 2013, Komodo Does Inc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Komodo Does Inc nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#ifndef SOCKET_H
#include "Socket.h"
#endif // SOCKET_H

namespace BsdSockets {

  /*
   * Forward Declarations
   */
  class Address;

  /** \class ClientSocket
   * 
   * \brief Client Socket providing ability to connect to and Address.
   *
   * This adds to Socket the ability to connetc() to an address.
   */
  class ClientSocket : public Socket {
  public:
    typedef std::shared_ptr<ClientSocket> Ptr;

    /** Closes socket.  Virtual destructor to support derived classes. */
    virtual ~ClientSocket();

    /** Create a ClientSocket to theAddress.
     *
     * \note The caller still needs to connect()
     *
     * @param theAddress to open and connect the ClientSocket to
     *
     * @return the ClientSocket
     */
    static ClientSocket::Ptr create(std::shared_ptr<Address> theAddress);

    /** Create and connect a ClientSocket to theAddress.
     *
     * @param theAddress to open the ClientSocket to
     *
     * @return the ClientSocket
     */
    static ClientSocket::Ptr open(std::shared_ptr<Address> theAddress);

  protected:
    /** Create ClientSocket to support an address, saving it for connect().
     *
     * @param theAddress to create socket to use
     */
    ClientSocket(std::shared_ptr<Address> theAddress);

  private:
    ClientSocket(const Socket& rhs) = delete;
    ClientSocket(Socket&&) = delete;
    ClientSocket& operator=(const Socket& rhs) = delete;
    ClientSocket& operator=(Socket&& rhs) = delete;

  public:
    /** Connect ClientSocket to the address. */
    void connect() const;
  };

} // namespace BsdSockets

#endif // CLIENT_SOCKET_H
