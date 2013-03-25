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
   *
   * \example ClientSocketExamples.cpp
   * Examples of using a ClientSocket
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
