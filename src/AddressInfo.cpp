#include <sys/socket.h>
#include <stdexcept>

#include <map>

#include "AddressInfo.h"

namespace BsdSockets {

  int socketDomainToRaw(SocketDomain domain) {
    static const std::map<SocketDomain, int> map =
      {
        {SocketDomain::INET4, PF_INET },
        {SocketDomain::INET6, PF_INET6 },
        {SocketDomain::LOCAL, PF_LOCAL },
        {SocketDomain::ROUTE, PF_ROUTE },
        {SocketDomain::KEY, PF_KEY },
        {SocketDomain::SYSTEM, PF_SYSTEM },
        {SocketDomain::RAW, PF_NDRV }
      };

    const auto found = map.find(domain);
    if(found == map.end()) {
      throw std::invalid_argument("Unknown SocketDomain");
    }

    return found->second;
  }

  SocketDomain rawToSocketDomain(int raw) {
    static const std::map<int, SocketDomain> map =
      {
        {PF_INET, SocketDomain::INET4 },
        {PF_INET6, SocketDomain::INET6 },
        {PF_LOCAL, SocketDomain::LOCAL },
        {PF_ROUTE, SocketDomain::ROUTE },
        {PF_KEY, SocketDomain::KEY },
        {PF_SYSTEM, SocketDomain::SYSTEM },
        {PF_NDRV, SocketDomain::RAW }
      };

    const auto found = map.find(raw);
    if(found == map.end()) {
      throw std::invalid_argument("Unknown SocketDomain");
    }

    return found->second;
  }


  int socketTypeToRaw(SocketType type) {
    static const std::map<SocketType, int> map =
      {
	{SocketType::STREAM, SOCK_STREAM},
	{SocketType::DATAGRAM, SOCK_DGRAM},
	{SocketType::RAW_PROTOCOL, SOCK_RAW},
	{SocketType::RELIABLY_DELIVERED, SOCK_RDM},
	{SocketType::SEQUENCED_PACKET, SOCK_SEQPACKET}
      };

    const auto found = map.find(type);
    if(found == map.end()) {
      throw std::invalid_argument("Unknown SocketType");
    }
    return found->second;
  }

} // namespace BsdSockets
