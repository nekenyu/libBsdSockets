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
