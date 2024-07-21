#pragma once

#include "esterv/crypto/block/carray.hpp"
namespace esterv::crypto::block
{

class Address : public C_Base<AddressType>
{
    c_array m_addrHash;
    static const QHash<AddressType, QString> JsonStr;

  protected:
    Address(const AddressType typ, const c_array addrHash);
    Address(const AddressType typ, QDataStream &in);
    Address(const AddressType typ, const QJsonValue &val);

  public:
    template <class from_type> static std::shared_ptr<const Address> from(from_type &val);

    static std::shared_ptr<const Address> NFT(const c_array addrhash);
    static std::shared_ptr<const Address> Account(const c_array addrhash);
    static std::shared_ptr<const Address> Ed25519(const c_array addrhash);
    static std::shared_ptr<const Address> Anchor(const c_array addrhash);

    [[nodiscard]] auto addrHash(void) const
    {
        return m_addrHash;
    }
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out << m_addrHash;
    }
    void addJson(QJsonObject &var) const override
    {
        C_Base::addJson(var);
        var.insert(JsonStr[m_type], m_addrHash.toHexString());
    }
};

class NFTAddress : public Address
{
    template <class... Args> NFTAddress(Args &&...args) : Address(AddressType::NFT, std::forward<Args>(args)...)
    {
    }
    friend class Address;
};
class AccountAddress : public Address
{
    template <class... Args> AccountAddress(Args &&...args) : Address(AddressType::Account, std::forward<Args>(args)...)
    {
    }
    friend class Address;
};
class AnchorAddress : public Address
{
    template <class... Args> AnchorAddress(Args &&...args) : Address(AddressType::Anchor, std::forward<Args>(args)...)
    {
    }
    friend class Address;
};
class Ed25519Address : public Address
{
    template <class... Args> Ed25519Address(Args &&...args) : Address(AddressType::Ed25519, std::forward<Args>(args)...)
    {
    }
    friend class Address;
};

} // namespace qiota::qblocks
