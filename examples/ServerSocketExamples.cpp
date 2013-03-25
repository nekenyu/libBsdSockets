
#include <sys/socket.h>

#include <memory>
#include <string>
#include <vector>

#include "InetAddress.h"
#include "ServerSocket.h"

using namespace BsdSockets;

void serverSocketExamples() {

  InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, "12345", "127.0.0.1");
  ServerSocket::Ptr server = ServerSocket::create(addr);
  server->bind();
  server->listen();

  // ...
  const int timeout_ms = 50;
  bool readyToRead = false;
  bool readyToWrite = false;
  bool readyWithError =false;
  server->select(timeout_ms, readyToRead, readyToWrite, readyWithError);
  if(readyToRead) {
    Socket::Ptr accepted = server->accept();
  }
}
