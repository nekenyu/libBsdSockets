#ifndef INET_ADDRESS_H

#ifndef ADDRESS_H
#include "Address.h"
#endif // ADDRESS_H

namespace BsdSockets {

  /*
   * Forward Declarations
   */
  class LowLevelAddress;
  class InetAddressPimpl;

  /** \class InetAddress
   *
   * \brief Internet Address Class supporting IPv4 and IPv6 and creatable from
   * hostnames and service names.
   *
   * Most users will create addresses with a hostname and either a service name
   * or port number.  This supports that with ease.  Additionally, create() can
   * accept a std::vector<InetAddress> to put all matching Addresses into.  For
   * example, some hostnames are served by multiple IP addresses to load
   * balance.  The caller can use create() to either return the first match or
   * populate a std::vector<InetAddress> with all matches.
   *
   * \example InetAddressExamples.cpp
   * Examples of creating InetAddress instances.
   */
  class InetAddress : public Address {
  public:
    typedef std::shared_ptr<InetAddress> Ptr;

    /** Virtual destructor to support derived classes */
    virtual ~InetAddress();

    /** Create an address with the given parameters putting all matches into
     * created.
     *
     * \note Either of address or serviceName may be empty, but not both.
     *
     * @param socketType the SocketType of the Address
     * @param serviceName name of standard service or port number desired, may be empty if address is not
     * @param address desired, may be empty if serviceName is not empty
     * @param[out] created the container to add created Addresses to
     * @param max the limit on the count of Addresses to match, 0 means unlimited matches
     *
     * @return count of Addresses added to created
     */
    static int create(SocketType socketType, const std::string& serviceName, const std::string& address,
		      std::vector<InetAddress::Ptr>& created, unsigned int max = 0);

    /** Create one address with the given parameters.
     *
     * This is a convenience method equivalent to:
     * \code
     * std::vector<InetAddress> found;
     * InetAddress::create(type, address, serviceName, found, 1);
     * return created.at(0);
     * \endcode
     *
     * \note Either of address or serviceName may be empty, but not both.
     *
     * @param socketType the SocketType of the Address
     * @param serviceName name of standard service or port number desired, may be empty if address is not
     * @param address desired, may be empty if serviceName is not empty
     *
     * @return InetAddress created from first match of address and serviceName
     */
    static InetAddress::Ptr create(SocketType socketType, const std::string& serviceName, const std::string& address = "");

  private:
    /** Internal method to instantiate with all const values from static method/
     *
     * @param theSocketDomain domain of the socket from the Address
     * @param theSocketType type of the socket from the Address
     * @param theProtocol protocol of the socket from the address
     * @param theServiceName requested by the user
     * @param theRequestedAddress used to create the address
     * @param pimpl private implementation of the class
     */
    InetAddress(SocketDomain theSocketDomain, SocketType theSocketType, int theProtocol,
		const std::string& theServiceName, const std::string& theRequestedAddress,
		std::shared_ptr<InetAddressPimpl> pimpl);

  private:
    InetAddress() = delete;
    InetAddress(const InetAddress& rhs) = delete;
    InetAddress(InetAddress&& rhs) = delete;
    InetAddress& operator=(const InetAddress& rhs) = delete;
    InetAddress& operator=(InetAddress&& rhs) = delete;

  public:
    /** @return the string address used to create this */
    const std::string& getRequestedAddress() const;

    /** @return the computed string address that was actually created.
     *
     * \note This may be an IP if a hostname was requested, etc.
     */
    const std::string& getActualAddress() const;

    /** @return the service name or port number requested for this address may be blank */
    const std::string& getServiceName() const;

    /** @return the actual port associated with this address */
    const unsigned int getPort() const;

  public:
    virtual std::shared_ptr<LowLevelAddress> makeTempLowLevelAddress() const;
    virtual Address::Ptr create(std::shared_ptr<LowLevelAddress> lowLevelAddress) const;
    virtual LowLevelAddress& getLowLevelAddress() const;

  private:
    /** The serviceName used to create this Address */
    const std::string serviceName;

    /** The string address used to create this */
    const std::string requestedAddress;

    /** The private implementation of this InetAddress */
    std::shared_ptr<InetAddressPimpl> const pimpl;
  };

}
// namespace BsdSockets

#endif // INET_ADDRESS_H
