/*

Copyright (c) 2013, Komodo Does Inc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Komodo Does Inc nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <sys/socket.h>
#include <poll.h>

#include <atomic>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <iostream>

#include "InetAddress.h"
#include "ServerSocket.h"

using namespace BsdSockets;

/** Echo everything on socket until a line starts with "quit" or "terminate"
 */
class CommandHandler {
public:
  /** Create with theSocket to echo on and greeting string
   *
   * @param theSocket to echo on
   * @param theTerimateRequested trigger to terminate the server from a client
   * @param greeting to echo at the beginning
   */
  CommandHandler(Socket::Ptr theSocket, std::atomic_bool& theTerminateRequested, const std::string& greeting)
    : socket(theSocket), terminateRequested(theTerminateRequested)
  {
    try {
      send(greeting);
      send("Any line beginning with \"quit\" will terminate this echo client.\n");
      send("Any line beginning with \"terminate\" will terminate the echo server.\n");
    } catch (...) {
      // Do nothing on error
    }
  }

  /** Run the echo without escaping any exceptions */
  void operator() () {
    try {
      while(!terminateRequested.load()) {
	if(readOne()) {
	  break;
	}
      }
    } catch(...) {
      // Do nothing on error
    }
  }

private:
  /** Read one or exit it if terminateRequested */
  bool readOne() {
    const SelectResult result = socket->select(timeout_ms, true, false, true);
    if(terminateRequested.load()) {
      return true;
    }

    if(result.readyWithError) {
      return true;
    }

    if(result.readyToRead) {
      return readCurrent();
    }
  }

  /** Read the next data, when ready and deal with it
   *
   * @return true if we are done, otherwise false to continue
   */
  bool readCurrent() {
    const int bufferLen = 1024;
    char buffer[bufferLen];
    
    const ssize_t readLength = socket->blockingReceive(buffer, bufferLen - 1);
    if(0 == readLength) {
      return false;
    }

    return actOn(std::string(buffer, readLength));
  }

  /** Act on the buffer
   *
   * @param buffer to act on
   *
   * @return true if we are done, otherwise false to continue
   */
  bool actOn(const std::string& buffer) {
    if(0 == buffer.find("terminate")) {
      send("Requesting shutdown now!\n");
      terminateRequested.store(true);
      return true;
    } else if(0 == buffer.find("quit")) {
      send("Goodbye!\n");
      return true;
    }

    send(buffer);
    return false;
  }

  /** Send a string message over the socket
   *
   * @param message to send
   */
  void send(const std::string& message) {
    socket->send(message.c_str(), message.length() + 1);
  }

private:
 /** Timeout for this socket */
  static const int timeout_ms = 500;

  /** Socket to echo */
  Socket::Ptr socket;

  /** Trigger to terminate the server from a client */
  std::atomic_bool& terminateRequested;
};

/** Opens a ServerSocket to accept connections to echo everything back on.
 */
class CommandServer {
public:
  /** Create bound to address
   *
   * @param address to open EchoServer
   */
  CommandServer(Address::Ptr address)
    : serverSocket(ServerSocket::open(address))
  {
    // Set the terminateRequested to false
    terminateRequested.store(false);
  }

  /** Run the echo without escaping any exceptions */
  void operator()() {
    try {
      // Loop over all incoming
      while(!terminateRequested.load()) {
	if(acceptOne()) {
	  break;
	}
      }
    } catch(...) {
      // Do nothing on error
    }
    
    // Loop over all threads to make sure they are done
    for(std::thread& acceptedThread : acceptedThreads) {
      try {
	acceptedThread.join();
      } catch (...) {
	// Do nothing on error
      }
    }
  }

private:
  /** Accept one or exit it if terminateRequested */
  bool acceptOne() {
    const SelectResult result = serverSocket->select(timeout_ms, true, false, true);
    if(terminateRequested.load()) {
      return true;
    }

    if(result.readyWithError) {
      return true;
    }

    if(result.readyToRead) {
      acceptCurrent();
    }

    return false;
  }

  /** Accept the next socket and spin off a CommandHandler */
  void acceptCurrent() {
    const int id = acceptedThreads.size() + 1;
    acceptedThreads.push_back(serverSocket->acceptInNewThread(
      [=](Socket::Ptr accepted) {
	const std::string greeting = std::string("Welcome, Connection #") + std::to_string(id) + "\n";
	CommandHandler e(accepted, terminateRequested, greeting);
	e();
      }));
  }

private:
  /** Timeout for this socket */
  static const int timeout_ms = 500;

  /** Socket we accept connections on */
  ServerSocket::Ptr serverSocket;

  /** All threads we have created so that we could join them */
  std::vector<std::thread> acceptedThreads;

  /** Trigger to terminate the server from a client */
  std::atomic_bool terminateRequested;
};

int main(int argc, char** argv) {
  InetAddress::Ptr addr = InetAddress::create(SocketType::STREAM, "11111", "127.0.0.1");

  // Run this server socket in its own thread so we could do something else...
  // This is superfluous for the example...
  std::thread serverThread(
  [=]() {
    CommandServer server(addr);
    server();
  });

  // Wait for server to return
  serverThread.join();

  return 0;
}

