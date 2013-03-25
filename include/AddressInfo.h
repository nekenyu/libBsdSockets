/*

Copyright (c) 2013, Komodo Does Inc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Komodo Does Inc nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef ADDRESS_INFO_H
#define ADDRESS_INFO_H

/**
 * BSD Sockets Package.
 */
namespace BsdSockets {

  /** The low-level domain of a socket or address.
   *
   * This provides detail to the low-level implementation.
   */
  enum class SocketDomain {
    /** Traditional internet address for TCP, UDP, etc. */
    INET4,

      /** IPv6 internet address TCP, UDP, etc. */
      INET6,

      /** Local/Unix/Pipe socket address */
      LOCAL,

      /** Internal Routing Protocol */
      ROUTE,

      /** Internal Key Management */
      KEY,

      /** Kernel Event Messages */
      SYSTEM,

      /** Raw Protocol Interface */
      RAW
      };

  /** The low-level type of the socket.
   *
   * This provides detail to the low-level implementation.
   */
  enum class SocketType {
    /** Stream of messages */
    STREAM,
      
      /** Datagram socket */
      DATAGRAM,

      /** Raw protocol interface */
      RAW_PROTOCOL,

      /** Reliably-delivered message */
      RELIABLY_DELIVERED,

      /** Sequenced packet stream */
      SEQUENCED_PACKET
      };

}

#endif // ADDRESS_INFO_H
