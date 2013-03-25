#include <stdexcept>
#include <system_error>
#include <memory>

#include <vector>
#include <string>

#include "InetAddress.h"
using namespace BsdSockets;

using namespace std;

void inetAddressExamples() {
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
}


