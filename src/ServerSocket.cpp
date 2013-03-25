#include <sys/socket.h>
#include <system_error>
#include <memory>

#include "ServerSocket.h"
#include "Address.h"
#include "LowLevelAddress.h"

namespace BsdSockets {

  ServerSocket::Ptr ServerSocket::create(Address::Ptr theAddress) {
    return ServerSocket::Ptr (new ServerSocket(theAddress));
  }

  ServerSocket::Ptr ServerSocket::open(Address::Ptr theAddress, int backlog) {
    ServerSocket::Ptr socket = create(theAddress);
    socket->bind();
    socket->listen(backlog);
    return socket;
  }

  ServerSocket::~ServerSocket() {
  }

  ServerSocket::ServerSocket(Address::Ptr theAddress)
    : Socket(theAddress)
  {
  }

  void ServerSocket::bind() const {
    const LowLevelAddress& lowLevelAddress = getAddress()->getLowLevelAddress();

    if(-1 == ::bind(getLowLevelSocket(), &lowLevelAddress.getSockAddr(), lowLevelAddress.getSockAddrLen())) {
      throw std::system_error(errno, std::system_category());
    }
  }

  void ServerSocket::listen(int backlog) const {
    if(-1 == ::listen(getLowLevelSocket(), backlog)) {
      throw std::system_error(errno, std::system_category());
    }
  }

  Socket::Ptr ServerSocket::accept() const {
    LowLevelAddress::Ptr tmp = getAddress()->makeTempLowLevelAddress();
    socklen_t len;

    const int socketCreated = ::accept(getLowLevelSocket(), &tmp->getSockAddr(), &len);
    if(len > tmp->getSockAddrLen()) {
      throw std::runtime_error("accept over-write the length of the sockaddr for the ServerSocket Address");
    }

    if(-1 == socketCreated) {
      throw std::system_error(errno, std::system_category());
    }
    if(AF_UNSPEC == tmp->getSockAddr().sa_family) {
      tmp->getSockAddr().sa_family = getAddress()->getLowLevelAddress().getSockAddr().sa_family;
    }
    
    Address::Ptr acceptedAddress = Address::Ptr(getAddress()->create(tmp));
    return Socket::create(acceptedAddress, socketCreated);
  }

} // namespace BsdSockets
