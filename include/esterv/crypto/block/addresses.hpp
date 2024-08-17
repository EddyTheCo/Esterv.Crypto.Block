#pragma once

#include "esterv/crypto/block/carray.hpp"
namespace esterv::crypto::block
{

class Address : public C_Base<AddressType>
{

  protected:
    Address(const AddressType typ) : C_Base{typ}
    {
    }

  public:
    template <class from_type> static std::shared_ptr<const Address> from(from_type &val);

    [[nodiscard]] static std::shared_ptr<const Address> NFT(const c_array addrhash);
    [[nodiscard]] static std::shared_ptr<const Address> Account(const c_array addrhash);
    [[nodiscard]] static std::shared_ptr<const Address> Ed25519(const c_array addrhash);
    [[nodiscard]] static std::shared_ptr<const Address> Anchor(const c_array addrhash);
    [[nodiscard]] static std::shared_ptr<const Address> Multi();

    [[nodiscard]] virtual c_array addrHash(void) const
    {
        return c_array();
    }
};

class HashAddress : virtual public Address
{
    c_array m_addrHash;
    QBLOCK_EXPORT static const QHash<AddressType, QString> JsonStr;

  protected:
    HashAddress(const AddressType typ, const c_array addrHash) : Address{typ}, m_addrHash{addrHash}
    {
    }
    HashAddress(const AddressType typ, QDataStream &in) : Address{typ}
    {
        m_addrHash = c_array(ByteSizes::hash, 0);
        in >> m_addrHash;
    }
    HashAddress(const AddressType typ, const QJsonValue &val)
        : Address{typ}, m_addrHash{val.toObject()[JsonStr.value(typ)]}
    {
    }

  public:
    [[nodiscard]] c_array addrHash(void) const override
    {
        return m_addrHash;
    }
    void setAddrHash(const c_array addrHash)
    {
        m_addrHash = addrHash;
    }
    void serialize(QDataStream &out) const override
    {
        Address::serialize(out);
        out << m_addrHash;
    }
    void addJson(QJsonObject &var) const override
    {
        Address::addJson(var);
        var.insert(JsonStr[m_type], m_addrHash.toHexString());
    }
};

class MultiAddress : public Address
{
    using WeightedAddress = std::pair<std::shared_ptr<const Address>, quint8>;
    struct CompareAddress
    {
        bool operator()(const WeightedAddress &p1, const WeightedAddress &p2) const
        {

            if (p1.first->addrHash() < p2.first->addrHash())
            {
                return true;
            }
            return false;
        }
    };

    std::set<WeightedAddress, CompareAddress> m_addresses;
    quint16 m_threshold;

    MultiAddress(const std::set<WeightedAddress, CompareAddress> &addresses, const quint16 &threshold)
        : Address{AddressType::Multi}, m_addresses{addresses}, m_threshold{threshold}
    {
    }
    MultiAddress(const QJsonValue &val) : Address{AddressType::Multi}, m_threshold{}
    {
    }
    MultiAddress(QDataStream &in) : Address{AddressType::Multi}
    {
        quint8 count;
        in >> count;
        for (auto i = 0; i < count; i++)
        {
            const auto address = Address::from(in);
            quint8 weight;
            in >> weight;
            m_addresses.insert({address, weight});
        }
        in >> m_threshold;
    }

    friend class Address;
};
class NFTAddress : public HashAddress
{
    template <class... Args> NFTAddress(Args &&...args) : HashAddress{AddressType::NFT, std::forward<Args>(args)...}
    {
    }
    friend class Address;
};
class AccountAddress : public HashAddress
{
    template <class... Args>
    AccountAddress(Args &&...args) : HashAddress{AddressType::Account, std::forward<Args>(args)...}
    {
    }
    friend class Address;
};
class AnchorAddress : public HashAddress
{
    template <class... Args>
    AnchorAddress(Args &&...args) : HashAddress{AddressType::Anchor, std::forward<Args>(args)...}
    {
    }
    friend class Address;
};
class Ed25519Address : public HashAddress
{
    template <class... Args>
    Ed25519Address(Args &&...args) : HashAddress{AddressType::Ed25519, std::forward<Args>(args)...}
    {
    }
    friend class Address;
};

} // namespace qiota::qblocks
