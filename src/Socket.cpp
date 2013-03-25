#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <poll.h>

#include <system_error>
#include <memory>

#include "Address.h"
#include "AddressInfoConversions.h"
#include "Socket.h"

namespace BsdSockets {

  Socket::~Socket() {
    if(-1 == lowLevelSocket) {
      return;
    }

    // Close it, ignoring the consequences since there is nothing that can be done
    close(lowLevelSocket);
  }

  Socket::Ptr Socket::create(Address::Ptr theAddress, int existingLowLevelSocket) {
    return Socket::Ptr(new Socket(theAddress, existingLowLevelSocket));
  }

  Socket::Socket(Address::Ptr theAddress)
    : lowLevelSocket(::socket(
		      socketDomainToLowLevel(theAddress->getSocketDomain()),
		      socketTypeToLowLevel(theAddress->getSocketType()),
		      theAddress->getProtocol()
		      )),
      address(theAddress)
  {
  }

  Socket::Socket(Address::Ptr theAddress, int existingLowLevelSocket)
    : lowLevelSocket(existingLowLevelSocket),
      address(theAddress)
  {
    if(-1 == lowLevelSocket) {
      throw std::system_error(errno, std::system_category());
    }
  }

  bool Socket::select(int timeout_ms, bool& readyToRead, bool& readyToWrite, bool& readyWithError) {
    struct timeval timeout;
    if(0 != timeout_ms) {
      timeout.tv_sec = timeout_ms / 1000;
      timeout.tv_usec = 1000 * (timeout_ms % 1000);
    }
  
    fd_set read;  FD_ZERO(&read);  FD_SET(lowLevelSocket, &read);
    fd_set write; FD_ZERO(&write); FD_SET(lowLevelSocket, &write);
    fd_set error; FD_ZERO(&error); FD_SET(lowLevelSocket, &error);
    const int result = ::select(lowLevelSocket + 1, &read, &write, &error, 0 == timeout_ms ? nullptr : &timeout);
    if(-1 == result) {
      throw std::system_error(errno, std::system_category());
    }

    readyToRead = FD_ISSET(lowLevelSocket, &read);
    readyToWrite = FD_ISSET(lowLevelSocket, &write);
    readyWithError = FD_ISSET(lowLevelSocket, &error);

    return readyToRead || readyToWrite || readyWithError;
  }

  short Socket::poll(int timeout_ms, short eventsToLookFor) {
     struct pollfd fds[1];
  
     fds[0].fd = lowLevelSocket;
     fds[0].events = eventsToLookFor;

    if(-1 == ::poll(fds, 1, timeout_ms)) {
      throw std::system_error(errno, std::system_category());
    }

    return fds[0].revents;
  }

  ssize_t Socket::send(const void* buffer, size_t length, int flags) const {
    const ssize_t result = ::send(lowLevelSocket, buffer, length, flags);
    if(-1 != result) {
      return result;
    }

    throw std::system_error(errno, std::system_category());
  }

  ssize_t Socket::receive(void* buffer, size_t length, int flags) const {
    const ssize_t result = ::recv(lowLevelSocket, buffer, length, flags);
    if(-1 != result) {
      return result;
    }

    throw std::system_error(errno, std::system_category());
  }

  void Socket::getSocketOption(int level, int optionName, void* optionValue, socklen_t* optionLength) const {
    if(-1 == ::getsockopt(lowLevelSocket, level, optionName, optionValue, optionLength)) {
      throw std::system_error(errno, std::system_category());
    }
  }

  void Socket::setSocketOption(int level, int optionName, const void* optionValue, socklen_t optionLength) const {
    if(-1 == ::setsockopt(lowLevelSocket, level, optionName, optionValue, optionLength)) {
      throw std::system_error(errno, std::system_category());
    }
  }

  std::shared_ptr<Address> Socket::getAddress() const {
    return address;
  }

  int Socket::getLowLevelSocket() const {
    return lowLevelSocket;
  }

} // namespace BsdSockets
