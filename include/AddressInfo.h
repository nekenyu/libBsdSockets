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

  /** Find the low-level constant for domain
   *
   * @param domain to get the low-level constant for
   *
   * @return the low-level constant for domain
   */
  int socketDomainToRaw(SocketDomain domain);

  /** Find the SocketDomain for the low-level value constant
   *
   * @param value to find the SocketDomain for
   *
   * @return the SocketDomain for the low-level value constant
   */
  SocketDomain rawToSocketDomain(int value);

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

  /** Find the low-level constant for type.
   *
   * @param type to get the low-level constant for
   *
   * @return the low-level constant for type
   */
  int socketTypeToRaw(SocketType type);

}

#endif // ADDRESS_INFO_H
