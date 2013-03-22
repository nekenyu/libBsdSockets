#ifndef ADDRESS_H
#define ADDRESS_H

#ifndef ADDRESS_INFO_H
#include "AddressInfo.h"
#endif // ADDRESS_INFO_H

namespace BsdSockets {

  /*
   * Forward Declarations
   */
  class RawAddress;
  
  /** Base class for all addresses used by libBsdSocket.
   *
   * Provides common information for all types of addresses and a pure-virtual
   * method to access the RawAddress.
   */
  class Address {
  public:
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
    /** Method for derived classes to provide access to the RawAddress information */
    virtual const RawAddress& getImpl() const = 0;

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
