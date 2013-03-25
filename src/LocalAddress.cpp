#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <memory>

#include <stdexcept>
#include <string>

#include "LocalAddress.h"

#include "LowLevelAddress.h"

namespace BsdSockets {
  typedef LowLevelAddressType<struct sockaddr_un> LocalLowLevelAddress;

  /** \brief The private implementation of LocalAddress.
   *
   * This provides the LowLevelAddress.
   */
  class LocalAddressPimpl : public LocalLowLevelAddress {
  public:
    /** Create by encoding thePath
     *
     * @param thePath to encode
     * @param allowBlank true if blank is allowed, which is really only from accept()
     */
    LocalAddressPimpl(const std::string& thePath, bool allowBlank)
    {
      if(!allowBlank && 0 == thePath.size()) {
	throw std::invalid_argument("thePath cannot be blank");
      }

      if(thePath.size() >= sizeof(lowLevelAddress.sun_path)) {
	throw std::invalid_argument("thePath is too long for this system");
      }

      lowLevelAddress.sun_family = AF_LOCAL;
      strcpy(lowLevelAddress.sun_path, thePath.c_str());
    }

  private:
    LocalAddressPimpl() = delete;
    LocalAddressPimpl(const LocalAddressPimpl& rhs) = delete;
    LocalAddressPimpl(LocalAddressPimpl&& rhs) = delete;
    LocalAddressPimpl& operator=(const LocalAddressPimpl& rhs) = delete;
    LocalAddressPimpl& operator=(LocalAddressPimpl&& rhs) = delete;
  };

  /*
   * LocalAddress Implementation
   */

  LocalAddress::Ptr LocalAddress::create(const std::string& thePath) {
    return LocalAddress::Ptr(new LocalAddress(thePath, false));
  }

  LocalAddress::~LocalAddress() {
  }

  LocalAddress::LocalAddress(const std::string& thePath, bool allowBlank)
    : Address(SocketDomain::LOCAL, SocketType::STREAM, 0),
      path(thePath), pimpl(new LocalAddressPimpl(thePath, allowBlank))
  {
    if(nullptr == pimpl) {
      throw new std::invalid_argument("Invalid path");
    }
  }

  const std::string& LocalAddress::getPath() const {
    return path;
  }

  LowLevelAddress::Ptr LocalAddress::makeTempLowLevelAddress() const {
    return LowLevelAddress::Ptr(new LocalLowLevelAddress());
  }

  Address::Ptr LocalAddress::create(std::shared_ptr<LowLevelAddress> lowLevelAddress) const {
    if(AF_LOCAL != lowLevelAddress->getSockAddr().sa_family) {
      throw std::invalid_argument("LowLevelAddress is of wrong type for LocalAddress to create");
    }

    const struct sockaddr_un& ref = *reinterpret_cast<const struct sockaddr_un*>(&lowLevelAddress->getSockAddr());
    return Address::Ptr(new LocalAddress(ref.sun_path, true));
  }

  LowLevelAddress& LocalAddress::getLowLevelAddress() const {
    return *pimpl;
  }

} // namespace BsdSockets


