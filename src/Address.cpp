#include <sys/socket.h>

#include "Address.h"

namespace BsdSockets {

  Address::~Address() {
  }

  Address::Address(SocketDomain theSocketDomain, SocketType theSocketType, int theProtocol)
    : socketDomain(theSocketDomain), socketType(theSocketType), protocol(theProtocol)
  {
  }

  Address::Address(const Address& rhs)
    : socketDomain(rhs.socketDomain), socketType(rhs.socketType), protocol(rhs.protocol)
  {
  }

  SocketDomain Address::getSocketDomain() const {
    return socketDomain;
  }

  SocketType Address::getSocketType() const {
    return socketType;
  }
  
  int Address::getProtocol() const {
    return protocol;
  }

} // namespace BsdSocket
