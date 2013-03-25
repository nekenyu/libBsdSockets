#ifndef ADDRESS_H
#define ADDRESS_H

#ifndef ADDRESS_INFO_H
#include "AddressInfo.h"
#endif // ADDRESS_INFO_H

namespace BsdSockets {

  /*
   * Forward Declarations
   */
  class LowLevelAddress;
  
  /** \class Address
   *
   * \brief Base class for all addresses.
   *
   * Provides common information for all types of addresses and a pure-virtual
   * method to access the LowLevelAddress.
   */
  class Address {
  public:
    typedef std::shared_ptr<Address> Ptr;

    /** Virtual destructor to support derived classes */
    virtual ~Address();
    
  protected:
    /** Create from parameters
     *
     * @param theSocketDomain the SocketDomain for the Address
     * @param theSocketType the SocketType for the Address
     * @param theProtocol the low-level protocol for the address
     */
    Address(SocketDomain theSocketDomain, SocketType theSocketType, int theProtocol);

    /** Copy from another Address
     *
     * @param rhs the Address to copy
     */
    Address(const Address& rhs);
    
  private:
    Address(Address&& rhs) = delete;
    Address& operator=(const Address& rhs) = delete;
    Address& operator=(Address&& rhs) = delete;

  public:
    /** @return the SocketDomain of the Address */
    SocketDomain getSocketDomain() const;

    /** @return the SocketType of the Address */
    SocketType getSocketType() const;

    /** @return the low-level protocol of the Address */
    int getProtocol() const;
    
  public:
    /** @return a LowLevelAddress suitable for create() */
    virtual std::shared_ptr<LowLevelAddress> makeTempLowLevelAddress() const = 0;

    /** Create a new Address from a LowLevelAddress using this Address as a template.
     *
     * @param lowLevelAddress data to create from
     *
     * @return Address created
     */
    virtual Address::Ptr create(std::shared_ptr<LowLevelAddress> lowLevelAddress) const = 0;
    
    /** Method for derived classes to provide access to the LowLevelAddress information */
    virtual const LowLevelAddress& getLowLevelAddress() const = 0;

  private:
    /** The SocketDomain of the Address */
    const SocketDomain socketDomain;
    
    /** The SocketType of the Address */
    const SocketType socketType;

    /** The low-level protocol of the Address */
    const int protocol;
  };

} // namespace BsdSockets

#endif // ADDRESS_H                          
