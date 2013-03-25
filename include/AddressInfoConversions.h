#ifndef ADDRESS_INFO_CONVERSIONS_H
#define ADDRESS_INFO_CONVERSIONS_H

#ifndef ADDRESS_INFO_H
#include "AddressInfo.h"
#endif // ADDRESS_INFO_H

/**
 * BSD Sockets Package.
 */
namespace BsdSockets {

  /** Find the low-level constant for domain
   *
   * @param domain to get the low-level constant for
   *
   * @return the low-level constant for domain
   */
  int socketDomainToLowLevel(SocketDomain domain);

  /** Find the SocketDomain for the low-level value constant
   *
   * @param value to find the SocketDomain for
   *
   * @return the SocketDomain for the low-level value constant
   */
  SocketDomain lowLevelToSocketDomain(int value);

  /** Find the low-level constant for type.
   *
   * @param type to get the low-level constant for
   *
   * @return the low-level constant for type
   */
  int socketTypeToLowLevel(SocketType type);

}

#endif // ADDRESS_INFO_CONVERSIONS_H
