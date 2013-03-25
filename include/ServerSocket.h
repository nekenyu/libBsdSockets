#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#ifndef SOCKET_H
#include "Socket.h"
#endif // SOCKET_H

namespace BsdSockets {

  /*
   * Forward Declarations
   */
  class Address;

  /** \class ServerSocket
   *
   * \brief Server Socket providing ability to bind, listen and accept connections
   *
   * \example ServerSocketExamples.cpp
   * Example of using a Server Socket
   *
   * \note Datagram sockets may send/receive with the ServerSocket directly.
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
     * @return the socket
     *
     * @see %listen() parameter backlog for more details
     */
    static ServerSocket::Ptr open(std::shared_ptr<Address> theAddress, int backlog = 128);

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
  };

} // namespace BsdSockets

#endif // SERVER_SOCKET_H
