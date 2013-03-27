/*

Copyright (c) 2013, Komodo Does Inc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Komodo Does Inc nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <stdexcept>
#include <system_error>
#include <memory>

#include <vector>
#include <string>

#include "InetAddress.h"
using namespace BsdSockets;

using namespace std;

int main(int argc, char** argv) {
  // Invalid service
  try {
    InetAddress::Ptr invalid = InetAddress::create(SocketType::STREAM, "9999999999");
  } catch (std::system_error e) {
  }

  // Specifying a service by name
  InetAddress::Ptr allSmtp = InetAddress::create(SocketType::STREAM, "smtp");

  // Specifying a service by port number
  InetAddress::Ptr allCustom = InetAddress::create(SocketType::STREAM, "12345");

  // Specifying IPv4 IP address
  InetAddress::Ptr localSmtp = InetAddress::create(SocketType::STREAM, "smtp", "127.0.0.1");

  // Specifying IPV4 IP address and port number
  InetAddress::Ptr localCustom = InetAddress::create(SocketType::STREAM, "12345", "127.0.0.1");

  // Specifying IPv6 IP address
  InetAddress::Ptr localInet6 = InetAddress::create(SocketType::STREAM, "smtp", "::1");

  // Collecting all matching addresses, such as the multiple IPs behind google.com
  std::vector<InetAddress::Ptr> google;
  InetAddress::create(SocketType::STREAM, "http", "google.com", google);

  return 0;
}
