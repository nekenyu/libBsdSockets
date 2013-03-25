
#include <memory>
#include <string>

#include "LocalAddress.h"
using namespace BsdSockets;

void localAddressExamples() {
  // Empty Address is invalid
  try {
    LocalAddress::Ptr invalid = LocalAddress::create("");
  } catch(...) {
  }

  // Too Long Address is invalid
  try {
    LocalAddress::Ptr invalid = LocalAddress::create(std::string().assign(256,'z'));
  } catch(...) {
  }

  // Valid address
  LocalAddress::Ptr valid = LocalAddress::create("./my-socket");
}
