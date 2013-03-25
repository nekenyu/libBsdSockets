#include <sys/socket.h>

#include <vector>
#include <string>
#include <memory>

#include "InetAddress.h"
#include "ClientSocket.h"

using namespace BsdSockets;

void clientSocketExamples() {
  InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, "12345", "127.0.0.1");
  ClientSocket::Ptr client = ClientSocket::create(addr);
  client->connect();
}
