#ifndef LOCAL_ADDRESS_H
#define LOCAL_ADDRESS_H

#ifndef ADDRESS_H
#include "Address.h"
#endif // ADDRESS_H

namespace BsdSockets {

  /*
   * Forward Declarations
   */
  class RawAddress;
  class LocalAddressPimpl;

  /** Local Socket Address (formerly Unix Domain Socket) Address representing
   * a named pipe's name.
   *
   * Example:
   * \code
   * const LocalAddress addr("foo");
   * \endcode
   */
  class LocalAddress : public Address {
  public:
    /** Virtual destructor to support derived classes */
    virtual ~LocalAddress();

    /** Create from thePath string
     *
     * @param thePath path to create from, cannot be empty, must be shorter 
     * than the unspecified maximum length for this system.
     */
    LocalAddress(const std::string& thePath);

    /** Create as copy of rhs
     *
     * @param rhs to copy
     */
    LocalAddress(const LocalAddress& rhs);

  private:
    LocalAddress() = delete;
    LocalAddress(LocalAddress&& rhs) = delete;
    LocalAddress& operator=(const LocalAddress& rhs) = delete;
    LocalAddress& operator=(LocalAddress&& rhs) = delete;

  public:
    /** @return the path of this LocalAddress */
    const std::string& getPath() const;

  public:
    virtual const RawAddress& getImpl() const;

  private:
    /** The path of this LocalAddress */
    const std::string path;

    /** The private implementation of this LocalAddress */
    const LocalAddressPimpl* const pimpl;
  };

} // namespace BsdSockets

#endif // LOCAL_ADDRESS_H
