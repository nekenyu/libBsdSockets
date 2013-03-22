#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#include <stdexcept>
#include <string>

#include "LocalAddress.h"

#include "RawAddress.h"

namespace BsdSockets {

  /** The private implementation of LocalAddress.
   *
   * This provides the RawAddress.
   */
  class LocalAddressPimpl : public RawAddress {
  public:
    /** Create by encoding thePath
     *
     * @param thePath to encode
     */
    LocalAddressPimpl(const std::string& thePath)
      : length(thePath.size() + sizeof(rawAddress.sun_family))
    {
      if(thePath.empty()) {
	throw std::invalid_argument("thePath cannot be blank");
      }

      if(thePath.size() >= sizeof(rawAddress.sun_path)) {
	throw std::invalid_argument("thePath is too long for this system");
      }

      rawAddress.sun_family = AF_LOCAL;
      strcpy(rawAddress.sun_path, thePath.c_str());
    }

    /** Create as copy of rhs.
     *
     * @param rhs to copy
     */
    LocalAddressPimpl(const LocalAddressPimpl& rhs)
      : length(rhs.length)
    {
      memcpy(&rawAddress, &rhs.rawAddress, sizeof(rawAddress));
    }

  private:
    LocalAddressPimpl() = delete;
    LocalAddressPimpl(LocalAddressPimpl&& rhs) = delete;
    LocalAddressPimpl& operator=(const LocalAddressPimpl& rhs) = delete;
    LocalAddressPimpl& operator=(LocalAddressPimpl&& rhs) = delete;

  public:
    virtual const struct sockaddr& getSockAddr() const {
      // This is ugly, but it wouldn't let me reinterpret cast without pointers
      // and I hate c-style casts...
      return *reinterpret_cast<const struct sockaddr*>(&rawAddress);
    }

    virtual socklen_t getSockLen() const {
      return length;
    }

  private:
    /** The length of the raw address */
    const int length;

    /** The low-level raw address */
    struct sockaddr_un rawAddress;
  };

  /*
   * LocalAddress Implementation
   */

  LocalAddress::~LocalAddress() {
    delete pimpl;
  }

  LocalAddress::LocalAddress(const LocalAddress& rhs)
    : Address(rhs),
      path(rhs.path), pimpl(new LocalAddressPimpl(*rhs.pimpl))
  {  
  }

  LocalAddress::LocalAddress(const std::string& thePath)
    : Address(SocketDomain::LOCAL, SocketType::STREAM, 0),
      path(thePath), pimpl(new LocalAddressPimpl(thePath))
  {
    if(nullptr == pimpl) {
      throw new std::invalid_argument("Invalid path");
    }
  }

  const std::string& LocalAddress::getPath() const {
    return path;
  }

  const RawAddress& LocalAddress::getImpl() const {
    return *pimpl;
  }

} // namespace BsdSockets


