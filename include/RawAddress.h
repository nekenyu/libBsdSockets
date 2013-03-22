#ifndef RAW_ADDRESS_H
#define RAW_ADDRESS_H

/*
 * Requires: #include <sys/socket.h>
 */

namespace BsdSockets {

  /** Interface for the raw address implementation.
   *
   * This class provides access to the raw sockaddr and socklen_t necessary for
   * low-level bsd socket implementations.  This is also available to the user
   * if they need it.
   *
   * This is an integral part of the Address class refactored to avoid requiring
   * the definition of the low level types in Address.h.
   */
  class RawAddress {
  public:
    /** Virtual destructor to support derived classes */
    virtual ~RawAddress() { }
    
  public:
    /** @return the low-level sockaddr the Address represents */
    virtual const struct sockaddr& getSockAddr() const = 0;

    /** @return the low-level length of the sockaddr the Address represents */
    virtual socklen_t getSockLen() const = 0;
  };

} // namespace BsdSockets

#endif // RAW_ADDRESS_H                          
