/*

Copyright (c) 2013, Komodo Does Inc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Komodo Does Inc nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

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
