/*

Copyright (c) 2013, Komodo Does Inc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Komodo Does Inc nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <memory>

#include <stdexcept>
#include <system_error>
#include <string>
#include <vector>

#include "AddressInfoConversions.h"
#include "InetAddress.h"

#include "LowLevelAddress.h"

namespace {
  /** Return actual address string from sockaddr info
   *
   * @param int family to use to decode
   * @param result struct addrinfo to calculate address string of
   *
   * @return string representation of address in result
   */
  std::string parseAddress(const int family, const struct sockaddr_in* const sin) {
    const size_t bufferLen = std::max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN);
    char buffer[bufferLen];
    buffer[0] = '\0';
    
    const char* actualAddressString = inet_ntop(family, &(sin->sin_addr.s_addr), buffer, bufferLen);
    if(nullptr == actualAddressString) {
      throw std::invalid_argument("Address not valid");
    }
    
    return actualAddressString;
  }

  /** Return actual address string from addrinfo
   *
   * @param result struct addrinfo to calculate address string of
   *
   * @return string representation of address in result
   */
  std::string parseAddress(const struct addrinfo& result) {
    const struct sockaddr_in* const sin = (sockaddr_in*) result.ai_addr;
    return parseAddress(result.ai_family, sin);
  }
}

namespace BsdSockets {
  typedef LowLevelAddressType<struct sockaddr_in>  InetLowLevelAddress;

  /** \brief The private implementation of InetAddress.
   *
   * This provides both the LowLevelAddress and the computed details of the Address
   * such as port number and actual address matched.
   *
   * The actual address may differ from the actual address due to hostname
   * resolution, etc.
   */
  class InetAddressPimpl : public InetLowLevelAddress {
  public:
    typedef std::shared_ptr<InetAddressPimpl> Ptr;

    /** Create with parameters
     * @param theLowLevelAddress to save a copy of for later use in getSockAddr()
     * @param theActualAddress the already formatted string version of the 
     * address in theLowLevelAddress
     */
    InetAddressPimpl(const sockaddr_in& theLowLevelAddress, const std::string& theActualAddress)
      : InetLowLevelAddress(theLowLevelAddress), actualAddress(theActualAddress)
    {
    }

  private:
    InetAddressPimpl() = delete;
    InetAddressPimpl(const InetAddressPimpl& rhs) = delete;
    InetAddressPimpl(InetAddressPimpl&& rhs) = delete;
    InetAddressPimpl& operator=(const InetAddressPimpl& rhs) = delete;
    InetAddressPimpl& operator=(InetAddressPimpl&& rhs) = delete;

  public:
    /** @return the actual address matched */
    const std::string& getActualAddress() const {
      return actualAddress;
    }

    /** @return the port or 0 for none */
    const unsigned int getPort() const {
      return ntohs(lowLevelAddress.sin_port);
    }

  private:
    /** The actual address matched */
    const std::string actualAddress;
  };

  /*
   * InetAddress implementation
   */

  int InetAddress::create(
    SocketType socketType, const std::string& serviceName, const std::string& address,
    std::vector<InetAddress::Ptr>& created, unsigned int max
  ) {
    // Setup hints to resolve address
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = socketTypeToLowLevel(socketType);
    
    // Resolve address and throw and handle errors including failing to match
    struct addrinfo *results = nullptr;
    const int error = getaddrinfo(address.c_str(), serviceName.c_str(), &hints, &results);
    if (error) {
      // TODO: this might not be appropriate to pass the error code here
      throw std::system_error(error, std::system_category(),  gai_strerror(error));
    }
    
    if(nullptr == results) {
      // Did not match
      throw std::invalid_argument("Invalid address and/or service");
    }

    // Loop over all results and push into created
    // Note, if max > 0, we stop after creating max InetAddresses
    int count = 0;
    for(struct addrinfo* result = results; nullptr != result && (max <= 0 || count < max); result = result->ai_next, ++count) {
      // Parse the socket domain and actual address
      const SocketDomain socketDomain = lowLevelToSocketDomain(result->ai_family);
      if(SocketDomain::INET4 != socketDomain && SocketDomain::INET6 != socketDomain) {
	throw std::invalid_argument("Invalid address family for address");
      }
      const std::string actualAddress = parseAddress(*result);

      // Create the pimpl and InetAddress
      InetAddressPimpl::Ptr pimpl(new InetAddressPimpl(
         *reinterpret_cast<const struct sockaddr_in* const>(result->ai_addr), actualAddress));
      created.push_back(InetAddress::Ptr(new InetAddress(socketDomain, socketType, result->ai_protocol, serviceName, address, pimpl)));
    }

    // Free results
    freeaddrinfo(results);

    return count;
  }

  InetAddress::Ptr InetAddress::create(SocketType socketType, const std::string& serviceName, const std::string& address) {
    // find at most 1...  if none are found exceptions will be thrown, so found.at(0) should be safe
    std::vector<InetAddress::Ptr> found;
    create(socketType, serviceName, address, found, 1);
    return found.at(0);
  }

  InetAddress::~InetAddress() {
  }

  InetAddress::InetAddress(SocketDomain theSocketDomain, SocketType theSocketType, int theProtocol,
			   const std::string& theServiceName, const std::string& theRequestedAddress,
			   std::shared_ptr<InetAddressPimpl> pimpl
  )
    : Address(theSocketDomain, theSocketType, theProtocol),
      requestedAddress(theRequestedAddress), serviceName(theServiceName),
      pimpl(pimpl)
  {
    if(nullptr == pimpl) {
      throw new std::invalid_argument("Invalid address");
    }
  }

  const std::string& InetAddress::getRequestedAddress() const {
    return requestedAddress;
  }

  const std::string& InetAddress::getActualAddress() const {
    return pimpl->getActualAddress();
  }

  const std::string& InetAddress::getServiceName() const {
    return serviceName;
  }

  const unsigned int InetAddress::getPort() const {
    return pimpl->getPort();
  }

  LowLevelAddress::Ptr InetAddress::makeTempLowLevelAddress() const {
    return LowLevelAddress::Ptr(new InetLowLevelAddress());
  }

  Address::Ptr InetAddress::create(std::shared_ptr<LowLevelAddress> fromLowLevelAddress) const {
    const int fromLowLevelAddressFamily = fromLowLevelAddress->getSockAddr().sa_family;

    if(AF_INET != fromLowLevelAddressFamily && AF_INET6 != fromLowLevelAddressFamily) {
      throw std::invalid_argument("LowLevelAddress is of wrong type for InetAddress to create");
    }

    const struct sockaddr_in* const sin = 
      reinterpret_cast<const struct sockaddr_in* const>(&fromLowLevelAddress->getSockAddr());

    const std::string theActualAddress = parseAddress(fromLowLevelAddressFamily, sin);
    InetAddressPimpl::Ptr pimpl(new InetAddressPimpl(*sin, theActualAddress));
    return InetAddress::Ptr(new InetAddress(getSocketDomain(), getSocketType(), getProtocol(), serviceName, "", pimpl));
  }
  
  LowLevelAddress& InetAddress::getLowLevelAddress() const {
    return *pimpl;
  }

} // namespace BsdSockets
