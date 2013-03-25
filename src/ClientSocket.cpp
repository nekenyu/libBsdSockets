#include <sys/socket.h>
#include <system_error>
#include <memory>

#include "ClientSocket.h"
#include "Address.h"
#include "LowLevelAddress.h"

namespace BsdSockets {

  ClientSocket::Ptr ClientSocket::create(Address::Ptr theAddress) {
    return ClientSocket::Ptr (new ClientSocket(theAddress));
  }

  ClientSocket::Ptr ClientSocket::open(Address::Ptr theAddress) {
    ClientSocket::Ptr socket = create(theAddress);
    socket->connect();
    return socket;
  }

  ClientSocket::ClientSocket(Address::Ptr theAddress)
    : Socket(theAddress)
  {
  }

  ClientSocket::~ClientSocket() {
  }

  void ClientSocket::connect() const {
    const LowLevelAddress& lowLevelAddress = getAddress()->getLowLevelAddress();

    if(-1 == ::connect(getLowLevelSocket(), &lowLevelAddress.getSockAddr(), lowLevelAddress.getSockAddrLen())) {
      throw std::system_error(errno, std::system_category());
    }
  }

} // namespace BsdSockets
