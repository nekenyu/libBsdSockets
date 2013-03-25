#ifndef SOCKET_H
#define SOCKET_H

#ifndef ADDRESS_INFO_H
#include "AddressInfo.h"
#endif // ADDRESS_INFO_H

namespace BsdSockets {

  /*
   * Forward Declarations
   */
  class Address;

  /** \class Socket
   *
   * \brief BSD Socket base class providing the basic, common functionality between
   * ServerSockets and ClientSockets.
   *
   * It provides creation (on creation), closing (on destruct), send/receive,
   * and socket option set/get.
   *
   * \note The socket is created during the constructor and closed in the
   * destructor.  The socket may not be closed prior.
   */
  class Socket {
  public:
    typedef std::shared_ptr<Socket> Ptr;

    /** Closes socket.  Virtual destructor to support derived classes. */
    virtual ~Socket();

    /** Create Socket from an existing low-level socket id.
     *
     * @param theAddress to create socket to work for
     * @param existingLowLevelSocket low-level socket id of an existing socket
     *
     * @return the socket
     */
    static Socket::Ptr create(std::shared_ptr<Address> theAddress, int existingLowLevelSocket);

  protected:
    /** Create Socket to support an address, reading parameters from address.
     *
     * @param theAddress to create socket to work for
     */
    Socket(std::shared_ptr<Address> theAddress);

  private:
    /** Create Socket from an existing low-level socket id.
     *
     * @param theAddress to create socket to work for
     * @param existingLowLevelSocket low-level socket id of an existing socket
     */
    Socket(std::shared_ptr<Address> theAddress, int existingLowLevelSocket);

    Socket(const Socket& rhs) = delete;
    Socket(Socket&&) = delete;
    Socket& operator=(const Socket& rhs) = delete;
    Socket& operator=(Socket&& rhs) = delete;

  public:
    /** Check state of socket without timeout_ms
     *
     * @param[out] timeout_ms timeout in milliseconds to stop trying to check
     * @param[out] readyToRead set true if ready to read, otherwise false
     * @param[out] readyToWrite set true if ready to write, otherwise false
     * @param[out] readyWithError set true if ready with error, otherwise false
     *
     * @return bool true if the socket is ready in any way, otherwise false
     */
    bool select(int timeout_ms, bool& readyToRead, bool& readyToWrite, bool& readyWithError);

    /** poll the Socket for eventsToLookFor timeout out after timeout_ms
     *
     * @param timeout_ms time in ms to try to find eventsToLookFor
     * @param eventsToLookFor low-level events from %poll() to look for
     *
     * @return low-level events from %poll() to look for
     */
    short poll(int timeout_ms, short eventsToLookFor);

  public:
    /** Send length bytes from buffer with flags to socket.
     *
     * @param buffer to read
     * @param length bytes to read from buffer and write to socket
     * @param flags to use when sending.
     *
     * \see %send() flags parameter for details of flags
     *
     * @return bytes sent
     */
    ssize_t send(const void* buffer, size_t length, int flags = 0) const;

    /** Read up to length bytes into buffer with flags from socket.
     *
     * @param buffer to write
     * @param length bytes to read from socket and write to buffer
     * @param flags to use when receiving.
     * 
     * \see %send() flags parameter for details of flags
     */
    ssize_t receive(void *buffer, size_t length, int flags = 0) const;

  public:
    /** Get socket option optionName at network level, saving to buffer
     * optionValue and to length optionLength.
     *
     * @param level network level of option
     * @param optionName name of the option at level
     * @param optionValue to save value to
     * @param optionLength to save length of optionValue to
     */
    void getSocketOption(int level, int optionName, void* optionValue, socklen_t* optionLength) const;

    /** Set socket option optionName at network level, reading from buffer
     * optionValue and from length optionLength.
     *
     * @param level network level of option
     * @param optionName name of the option at level
     * @param optionValue to read value from
     * @param optionLength length of optionValue
     */
    void setSocketOption(int level, int optionName, const void* optionValue, socklen_t optionLength) const;

  public:
    /** @return the low-level socket number */
    int getLowLevelSocket() const;

    /** @return the address of this socket */
    std::shared_ptr<Address> getAddress() const;

  private:
    /** The low-level socket number */
    const int lowLevelSocket;

    /** The address of this socket */
    std::shared_ptr<Address> address;
  };

} // namespace BsdSockets

#endif // SOCKET_H
