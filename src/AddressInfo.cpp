#include <sys/socket.h>
#include <stdexcept>

#include "AddressInfo.h"

namespace BsdSockets {

  int socketDomainToRaw(SocketDomain domain) {
    if(SocketDomain::INET4 == domain) { return PF_INET; }
    else if(SocketDomain::INET6 == domain) { return PF_INET6; }
    else if(SocketDomain::LOCAL == domain) { return PF_LOCAL; }
    else if(SocketDomain::ROUTE == domain) { return PF_ROUTE; }
    else if(SocketDomain::KEY == domain) { return PF_KEY; }
    else if(SocketDomain::SYSTEM == domain) { return PF_SYSTEM; }
    else if(SocketDomain::RAW == domain) { return PF_NDRV; }
    else throw std::invalid_argument("Unknown SocketDomain");
  }

  SocketDomain rawToSocketDomain(int raw) {
    if(PF_INET == raw) { return SocketDomain::INET4; }
    else if(PF_INET6 == raw) { return SocketDomain::INET6; }
    else if(PF_LOCAL == raw) { return SocketDomain::LOCAL; }
    else if(PF_ROUTE == raw) { return SocketDomain::ROUTE; }
    else if(PF_KEY == raw) { return SocketDomain::KEY; }
    else if(PF_SYSTEM == raw) { return SocketDomain::SYSTEM; }
    else if(PF_NDRV == raw) { return SocketDomain::RAW; }
    else throw std::invalid_argument("Unknown SocketDomain");
  }


  int socketTypeToRaw(SocketType type) {
    if(SocketType::STREAM == type) { return SOCK_STREAM; }
    else if(SocketType::DATAGRAM == type) { return SOCK_DGRAM; }
    else if(SocketType::RAW_PROTOCOL == type) { return SOCK_RAW; }
    else if(SocketType::RELIABLY_DELIVERED == type) { return SOCK_RDM; }
    else if(SocketType::SEQUENCED_PACKET == type) { return SOCK_SEQPACKET; }
    else throw std::invalid_argument("Unknown SocketType");
  }

} // namespace BsdSockets
