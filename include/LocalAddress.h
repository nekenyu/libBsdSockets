#ifndef LOCAL_ADDRESS_H
#define LOCAL_ADDRESS_H

#ifndef ADDRESS_H
#include "Address.h"
#endif // ADDRESS_H

namespace BsdSockets {

  /*
   * Forward Declarations
   */
  class LowLevelAddress;
  class LocalAddressPimpl;

  /** \class LocalAddress
   *
   * \brief Local Socket Address (formerly Unix Domain Socket) Address representing
   * a named pipe's name.
   *
   * \example LocalAddressExamples.cpp
   * Examples of using a LocalAddress
   */
  class LocalAddress : public Address {
  public:
    typedef std::shared_ptr<LocalAddress> Ptr;

    /** Virtual destructor to support derived classes */
    virtual ~LocalAddress();

    /** Create from thePath string
     *
     * @param thePath path to create from, cannot be empty, must be shorter 
     * than the unspecified maximum length for this system.
     *
     * @return LocalAddress created
     */
    static LocalAddress::Ptr create(const std::string& thePath);

  private:
    /** Create from thePath string
     *
     * @param thePath path to create from, cannot be empty, must be shorter 
     * than the unspecified maximum length for this system.
     * @param a blank address is allowed only from accept()
     */
    LocalAddress(const std::string& thePath, bool allowBlank = false);

  private:
    LocalAddress() = delete;
    LocalAddress(const LocalAddress& rhs) = delete;
    LocalAddress(LocalAddress&& rhs) = delete;
    LocalAddress& operator=(const LocalAddress& rhs) = delete;
    LocalAddress& operator=(LocalAddress&& rhs) = delete;

  public:
    /** @return the path of this LocalAddress */
    const std::string& getPath() const;

  public:
    virtual std::shared_ptr<LowLevelAddress> makeTempLowLevelAddress() const;
    virtual Address::Ptr create(std::shared_ptr<LowLevelAddress> lowLevelAddress) const;
    virtual LowLevelAddress& getLowLevelAddress() const;

  private:
    /** The path of this LocalAddress */
    const std::string path;

    /** The private implementation of this LocalAddress */
    std::shared_ptr<LocalAddressPimpl> pimpl;
  };

} // namespace BsdSockets

#endif // LOCAL_ADDRESS_H
