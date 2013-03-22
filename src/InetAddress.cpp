#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include <stdexcept>
#include <system_error>
#include <string>
#include <vector>

#include "InetAddress.h"

#include "RawAddress.h"

namespace {
  /** Return actual address string from sockaddr info
   *
   * @param result struct addrinfo to calculate address string of
   *
   * @return string representation of address in result
   */
  std::string parseAddress(const struct addrinfo& result) {
    const size_t bufferLen = std::max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN);
    char buffer[bufferLen];
    buffer[0] = '\0';
    
    struct sockaddr_in* sin = (sockaddr_in*) result.ai_addr;
    const char* actualAddressString = inet_ntop(result.ai_family, &(sin->sin_addr.s_addr), buffer, bufferLen);
    if(nullptr == actualAddressString) {
      throw std::invalid_argument("Address not valid");
    }
    
    return actualAddressString;
  }
}

namespace BsdSockets {

  /** The private implementation of InetAddress.
   *
   * This provides both the RawAddress and the computed details of the Address
   * such as port number and actual address matched.
   *
   * The actual address may differ from the actual address due to hostname
   * resolution, etc.
   */
  class InetAddressPimpl : public RawAddress {
  public:
    /** Create with parameters
     * @param theRawAddress to save a copy of for later use in getSockAddr()
     * @param theActualAddress the already formatted string version of the 
     * address in theRawAddress
     */
    InetAddressPimpl(const sockaddr_in* const theRawAddress, const std::string& theActualAddress)
      : actualAddress(theActualAddress)
    {
      memcpy(&rawAddress, theRawAddress, sizeof(rawAddress));
    }

    /** Create as copy of rhs.
     *
     * @param rhs to copy
     */
    InetAddressPimpl(const InetAddressPimpl& rhs)
    : actualAddress(rhs.actualAddress)
    {
      memcpy(&rawAddress, &rhs.rawAddress, sizeof(rawAddress));
    }

  private:
    InetAddressPimpl() = delete;
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
      return ntohs(rawAddress.sin_port);
    }

  public:
    virtual const struct sockaddr& getSockAddr() const {
      return (const struct sockaddr&) rawAddress;
    }

    virtual socklen_t getSockLen() const {
      return sizeof(rawAddress);
    }

  private:
    /** The low-level raw address */
    struct sockaddr_in rawAddress;

    /** The actual address matched */
    const std::string actualAddress;
  };

  /*
   * InetAddress implementation
   */

  int InetAddress::create(
    SocketType socketType, const std::string& address, const std::string& serviceName,
    std::vector<InetAddress>& created, unsigned int max
  ) {
    // Setup hints to resolve address
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = socketTypeToRaw(socketType);
    
    // Resolve address and throw and handle errors including failing to match
    struct addrinfo *results = nullptr;
    const int error = getaddrinfo(address.c_str(), serviceName.c_str(), &hints, &results);
    if (error) {
      // TODO: this might not be appropriate
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
      const SocketDomain socketDomain = rawToSocketDomain(result->ai_family);
      if(SocketDomain::INET4 != socketDomain && SocketDomain::INET6 != socketDomain) {
	throw std::invalid_argument("Invalid address family for address");
      }
      const std::string actualAddress = parseAddress(*result);

      // Create the pimpl and InetAddress
      const InetAddressPimpl* const pimpl = new InetAddressPimpl(reinterpret_cast<const struct sockaddr_in* const>(result->ai_addr), actualAddress);
      created.push_back(InetAddress(socketDomain, socketType, result->ai_protocol, address, serviceName, pimpl));
    }

    // Free results
    freeaddrinfo(results);

    return count;
  }

  InetAddress InetAddress::create(SocketType socketType, const std::string& address, const std::string& serviceName) {
    // find at most 1...  if none are found exceptions will be thrown, so found.at(0) should be safe
    std::vector<InetAddress> found;
    create(socketType, address, serviceName, found, 1);
    return found.at(0);
  }

  InetAddress::~InetAddress() {
    delete pimpl;
  }

  InetAddress::InetAddress(const InetAddress& rhs)
    : Address(rhs),
      requestedAddress(rhs.requestedAddress), serviceName(rhs.serviceName),
      pimpl(new InetAddressPimpl(*rhs.pimpl))
  {  
  }

  InetAddress::InetAddress(InetAddress&& rhs)
    : Address(rhs),
      requestedAddress(rhs.requestedAddress), serviceName(rhs.serviceName),
      pimpl(new InetAddressPimpl(*rhs.pimpl))
  {
    // Note: This is really copy not move, since move requires nullptring rhs.pimpl, which is const...
  }

  InetAddress::InetAddress(SocketDomain theDomain, SocketType theSocketType, int theProtocol,
		const std::string& theRequestedAddress, const std::string& theServiceName,
		const InetAddressPimpl* const pimpl
  )
    : Address(theDomain, theSocketType, theProtocol),
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

  const RawAddress& InetAddress::getImpl() const {
    return *pimpl;
  }

} // namespace BsdSockets
