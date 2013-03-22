#include <stdexcept>
#include <system_error>

#include <vector>
#include <string>

#include "InetAddress.h"
using namespace BsdSockets;

using namespace std;

void inetAddressExamples() {
  // Invalid service
  try {
    const InetAddress invalid = InetAddress::create(SocketType::STREAM, "", "9999999999");
  } catch (std::system_error e) {
  }

  // Specifying a service by name
  const InetAddress allSmtp = InetAddress::create(SocketType::STREAM, "", "smtp");

  // Specifying a service by port number
  const InetAddress allCustom = InetAddress::create(SocketType::STREAM, "", "12345");

  // Specifying IPv4 IP address
  const InetAddress localSmtp = InetAddress::create(SocketType::STREAM, "127.0.0.1", "smtp");

  // Specifying IPV4 IP address and port number
  const InetAddress localCustom = InetAddress::create(SocketType::STREAM, "127.0.0.1", "12345");

  // Specifying IPv6 IP address
  const InetAddress localInet6 = InetAddress::create(SocketType::STREAM, "::1", "");

  // Collecting all matching addresses, such as the multiple IPs behind google.com
  std::vector<InetAddress> google;
  InetAddress::create(SocketType::STREAM, "google.com", "", google);
}


