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
#include <stdexcept>

#include <map>

#include "AddressInfoConversions.h"

namespace BsdSockets {

  int socketDomainToLowLevel(SocketDomain domain) {
    static const std::map<SocketDomain, int> map =
      {
        {SocketDomain::INET4, PF_INET },
        {SocketDomain::INET6, PF_INET6 },
        {SocketDomain::LOCAL, PF_LOCAL },
        {SocketDomain::ROUTE, PF_ROUTE },
        {SocketDomain::KEY, PF_KEY },
        {SocketDomain::SYSTEM, PF_SYSTEM },
        {SocketDomain::RAW, PF_NDRV }
      };

    const auto found = map.find(domain);
    if(found == map.end()) {
      throw std::invalid_argument("Unknown SocketDomain");
    }

    return found->second;
  }

  SocketDomain lowLevelToSocketDomain(int lowLevel) {
    static const std::map<int, SocketDomain> map =
      {
        {PF_INET, SocketDomain::INET4 },
        {PF_INET6, SocketDomain::INET6 },
        {PF_LOCAL, SocketDomain::LOCAL },
        {PF_ROUTE, SocketDomain::ROUTE },
        {PF_KEY, SocketDomain::KEY },
        {PF_SYSTEM, SocketDomain::SYSTEM },
        {PF_NDRV, SocketDomain::RAW }
      };

    const auto found = map.find(lowLevel);
    if(found == map.end()) {
      throw std::invalid_argument("Unknown SocketDomain");
    }

    return found->second;
  }


  int socketTypeToLowLevel(SocketType type) {
    static const std::map<SocketType, int> map =
      {
	{SocketType::STREAM, SOCK_STREAM},
	{SocketType::DATAGRAM, SOCK_DGRAM},
	{SocketType::RAW_PROTOCOL, SOCK_RAW},
	{SocketType::RELIABLY_DELIVERED, SOCK_RDM},
	{SocketType::SEQUENCED_PACKET, SOCK_SEQPACKET}
      };

    const auto found = map.find(type);
    if(found == map.end()) {
      throw std::invalid_argument("Unknown SocketType");
    }
    return found->second;
  }

} // namespace BsdSockets
