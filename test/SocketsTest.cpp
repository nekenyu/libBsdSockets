#include <cppunit/extensions/HelperMacros.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>

#include <stdexcept>
#include <system_error>
#include <string>
#include <sstream>

#include "InetAddress.h"
#include "LocalAddress.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

using namespace BsdSockets;

class SocketsTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(SocketsTest);
  CPPUNIT_TEST_EXCEPTION(clientWithoutServer, std::system_error);
  CPPUNIT_TEST(clientServerSimpleExchange);
  CPPUNIT_TEST(clientServerSelectNoConnection);
  CPPUNIT_TEST(clientServerSelectAccept);
  CPPUNIT_TEST(clientServerSelectReadAccepted);
  CPPUNIT_TEST(clientServerPollNoConnection);
  CPPUNIT_TEST(clientServerPollAccept);
  CPPUNIT_TEST(clientServerPollReadAccepted);
  CPPUNIT_TEST(clientServerPollReadClient);
  CPPUNIT_TEST(localSelectAll);
  CPPUNIT_TEST_SUITE_END();

private:
  static int lastPort;
  const int timeout_ms = 500;
  std::string port;

public:
  void setUp() {
    ++lastPort;
    std::ostringstream s;
    s << lastPort;
    port = s.str();
  }

  void tearDown() {
  }

  void assertSelect(Socket::Ptr socket,
		    const bool readyToReadExpected, const bool readyToWriteExpected, const bool readyWithErrorExpected,
		    const std::string& mesg
  ) {
    bool readyToRead = false;
    bool readyToWrite = false;
    bool readyWithError = false;

    const bool ready = 0 < socket->select(timeout_ms, readyToRead, readyToWrite, readyWithError);
    CPPUNIT_ASSERT_MESSAGE(mesg, ready == readyToRead || readyToWrite || readyWithError);
    CPPUNIT_ASSERT_MESSAGE(mesg, readyWithErrorExpected == readyWithError);
    CPPUNIT_ASSERT_MESSAGE(mesg, readyToReadExpected == readyToRead);
    CPPUNIT_ASSERT_MESSAGE(mesg, readyToWriteExpected == readyToWrite);
  }

  void assertPoll(Socket::Ptr socket,
		  const bool readyToReadExpected, const bool readyToWriteExpected, const bool readyWithErrorExpected,
		  const std::string& mesg
  ) {
    const short eventsToLookFor = POLLRDNORM | POLLWRNORM;
    const short eventsFound = socket->poll(timeout_ms, eventsToLookFor);
    CPPUNIT_ASSERT_MESSAGE(mesg, readyWithErrorExpected == (0 != (eventsFound & POLLERR)));
    CPPUNIT_ASSERT_MESSAGE(mesg, readyToReadExpected == (0 != (eventsFound & POLLRDNORM)));
    CPPUNIT_ASSERT_MESSAGE(mesg, readyToWriteExpected == (0 != (eventsFound & POLLWRNORM)));
  }


  void assertSelectOrPoll(Socket::Ptr socket, const bool doSelect,
		    const bool readyToReadExpected, const bool readyToWriteExpected, const bool readyWithErrorExpected,
		    const std::string& mesg
  ) {
    if(doSelect) {
      assertSelect(socket, readyToReadExpected, readyToWriteExpected, readyWithErrorExpected, mesg);
    } else {
      assertPoll(socket, readyToReadExpected, readyToWriteExpected, readyWithErrorExpected, mesg);
    }
  }

  void clientWithoutServer() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, "127.0.0.1", port);
    ClientSocket::Ptr client = ClientSocket::open(address);
  }

  void parameterizedClientServerExchange(Address::Ptr address, bool doAccept, bool doSelect, bool noConnections, bool accept, bool readAccepted, bool readClient) {
    const SocketDomain domain = address->getSocketDomain();
    const bool isNetwork = SocketDomain::INET4 == domain || SocketDomain::INET6 == domain;

    // Create Server
    ServerSocket::Ptr server = ServerSocket::open(address, 16);

    // Create Client
    if(noConnections) {
      assertSelectOrPoll(server, doSelect, false, false, false, "No server connections waiting");
    }
    ClientSocket::Ptr client = ClientSocket::open(address);

    // Network Options
    if(isNetwork) {
      int flag = 0;
      socklen_t length;

      // TCP_NODELAY initially 0
      client->getSocketOption(IPPROTO_TCP, TCP_NODELAY, (char*) &flag, &length);
      CPPUNIT_ASSERT_MESSAGE("TCP_NODELAY initial length sizeof(int)", sizeof(flag) == length);
      CPPUNIT_ASSERT_MESSAGE("TCP_NODELAY initial value 0", 0 == flag);

      // TCP_NODELAY set to 1
      flag = 1;
      client->setSocketOption(IPPROTO_TCP, TCP_NODELAY, (char*) &flag, sizeof(flag));
    }

    // Accept client
    if(doAccept && accept && isNetwork) {
      assertSelectOrPoll(server, doSelect, true, false, false, "Client connected");
    }
    Socket::Ptr accepted = doAccept ? server->accept() : server;
    CPPUNIT_ASSERT(accepted);

    //
    // Messaging
    //
    const std::string msg1 = "Hello!";
    const size_t bufLen = 1024;
    char buffer[bufLen];

    // client -> accepted
    CPPUNIT_ASSERT(msg1.length() == client->send(msg1.c_str(), msg1.length()));

    if(readAccepted) {
      assertSelectOrPoll(accepted, doSelect, true, true, false, "Client sent");
    }
    CPPUNIT_ASSERT(msg1.length() == accepted->receive(buffer, bufLen));
    CPPUNIT_ASSERT(msg1 == buffer);

    // accepted -> client
    CPPUNIT_ASSERT(msg1.length() == accepted->send(buffer, msg1.length()));

    if(readClient) {
      assertSelectOrPoll(client, doSelect, true, true, false, "Accepted sent");
    }
    CPPUNIT_ASSERT(msg1.length() == client->receive(buffer, bufLen));
    CPPUNIT_ASSERT(msg1 == buffer);
  }

  void clientServerSimpleExchange() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, false, false, false, false, false);
  }

  void clientServerSelectNoConnection() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, true, true, false, false, false);
  }

  void clientServerSelectAccept() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, true, false, true, false, false);
  }

  void clientServerSelectReadAccepted() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, true, false, false, true, false);
  }

  void clientServerSelectReadClient() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, true, false, false, false, true);
  }

  void clientServerSelectAll() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, true, true, true, true, true);
  }

  void clientServerPollNoConnection() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, false, true, false, false, false);
  }

  void clientServerPollAccept() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, false, false, true, false, false);
  }

  void clientServerPollReadAccepted() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, false, false, false, true, false);
  }

  void clientServerPollReadClient() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, false, false, false, false, true);
  }

  void clientServerPollAll() {
    Address::Ptr address = InetAddress::create(SocketType::STREAM, port, "127.0.0.1");
    parameterizedClientServerExchange(address, true, false, true, true, true, true);
  }

  void removeLocalSocket(const std::string& path) {
    // Remove the local socket, ignore if it fails.... it will be caught on ServerSocket::open
    try {
      unlink(port.c_str());
    } catch (...) { }
  }

  void localSelectAll() {
    port = std::string("./local-socket-") + port;
    Address::Ptr address = LocalAddress::create(port);
    
    removeLocalSocket(port);
    try {
      parameterizedClientServerExchange(address, true, true, true, true, true, true);
    } catch(...) {
      removeLocalSocket(port);
      throw;
    }
    removeLocalSocket(port);
  }
};

int SocketsTest::lastPort = 32000;

CPPUNIT_TEST_SUITE_REGISTRATION(SocketsTest);
