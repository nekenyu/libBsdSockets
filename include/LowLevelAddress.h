#ifndef LOW_LEVEL_ADDRESS_H
#define LOW_LEVEL_ADDRESS_H

/*
 * Requires: #include <sys/socket.h>
 */

namespace BsdSockets {

  /** \class LowLevelAddress
   *
   * \brief Interface for the low-level address implementation.
   *
   * This class provides access to the low-level sockaddr and socklen_t necessary for
   * low-level bsd socket implementations.  This is also available to the user
   * if they need it.
   *
   * This is an integral part of the Address class refactored to avoid requiring
   * the definition of the low level types in Address.h.
   */
  class LowLevelAddress {
  public:
    typedef std::shared_ptr<LowLevelAddress> Ptr;

    /** Virtual destructor to support derived classes */
    virtual ~LowLevelAddress() { }
    
  public:
    /** @return the low-level sockaddr the Address represents */
    virtual const struct sockaddr& getSockAddr() const = 0;

    /** @return the low-level sockaddr the Address represents */
    virtual struct sockaddr& getSockAddr() = 0;

    /** @return the length of the low-level the sockaddr the Address represents */
    virtual socklen_t getSockAddrLen() const = 0;
  };

  /** \class LowLevelAddressType
   *
   * \brief LowLevelAddress template for the various cast-to-sockaddr implementaitons.
   *
   * This is not required, but provides a convenient template to get the job done.
   */
  template <class Impl>
    class LowLevelAddressType : public LowLevelAddress {
  public:
    typedef std::shared_ptr<LowLevelAddressType> Ptr;
    
    /** Virtual destructor to support derived classes */
    virtual ~LowLevelAddressType() { }

    /** Create with a zeroed low-level data */
    LowLevelAddressType() {
      memset(&lowLevelAddress, 0, sizeof(lowLevelAddress));
    }
    
    /** Create as a copy of theLowLevelAddress */
    LowLevelAddressType(const Impl& theLowLevelAddress) {
      memcpy(&lowLevelAddress, &theLowLevelAddress, sizeof(lowLevelAddress));
    }

    /** Create as a copy of rhs */
    LowLevelAddressType(const LowLevelAddressType& rhs) {
      memcpy(&lowLevelAddress, &rhs.lowLevelAddress, sizeof(lowLevelAddress));
    }

  private:
    LowLevelAddressType(LowLevelAddressType&& rhs) = delete;
    LowLevelAddressType& operator=(const LowLevelAddressType& rhs) = delete;
    LowLevelAddressType& operator=(LowLevelAddressType&& rhs) = delete;

  public:
    virtual const struct sockaddr& getSockAddr() const {
      // This is ugly, but it wouldn't let me reinterpret cast without pointers
      // and I hate c-style casts...
      return *reinterpret_cast<const struct sockaddr*>(&lowLevelAddress);
    }

    virtual struct sockaddr& getSockAddr() {
      // This is ugly, but it wouldn't let me reinterpret cast without pointers
      // and I hate c-style casts...
      return *reinterpret_cast<struct sockaddr*>(&lowLevelAddress);
    }

    virtual socklen_t getSockAddrLen() const {
      return sizeof(lowLevelAddress);
    }

  public:
    /** The low-level address */
    Impl lowLevelAddress;
  };

} // namespace BsdSockets

#endif // LOW_LEVEL_ADDRESS_H
