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
    [[nodiscard]] static std::shared_ptr<const Address> Multi(
        const std::initializer_list<std::pair<std::shared_ptr<const Address>, quint8>> &weightedAddresses,
        const quint16 &threshold);

    [[nodiscard]] virtual c_array addrHash(void) const
    {
        return c_array();
    }
    [[nodiscard]] c_array addr(void) const
    {
        c_array serialAddr = c_array(1, static_cast<char>(m_type));
        serialAddr.append(addrHash());
        return serialAddr;
    }
};

class HashAddress : public Address
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
    MultiAddress(const QJsonValue &val)
        : Address{AddressType::Multi}, m_threshold{static_cast<quint16>(val.toObject()["threshold"].toString().toInt())}
    {
        const auto addresses = val.toObject()["addresses"].toArray();
        for (const auto &v : addresses)
        {
            const auto jsonAddress = v.toObject()["address"];
            const auto address = Address::from(jsonAddress);
            const auto weight = static_cast<quint8>(v.toObject()["weight"].toInt());
            m_addresses.insert({address, weight});
        }
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

  public:
    [[nodiscard]] virtual c_array addrHash(void) const override
    {
        c_array serialAddr;
        auto buffer = QDataStream(&serialAddr, QIODevice::WriteOnly | QIODevice::Append);
        buffer.setByteOrder(QDataStream::LittleEndian);
        serialize(buffer);
        return QCryptographicHash::hash(serialAddr, QCryptographicHash::Blake2b_256);
    }
    [[nodiscard]] auto threshold(void) const
    {
        return m_threshold;
    }
    void setThreshold(const quint16 threshold)
    {
        if (threshold > 0)
        {
            m_threshold = threshold;
        }
    }
    void serialize(QDataStream &out) const override
    {
        Address::serialize(out);
        quint8 count = m_addresses.size();
        out << count;
        for (const auto &v : m_addresses)
        {
            if (v.first)
            {
                v.first->serialize(out);
                out << v.second;
            }
        }
        out << m_threshold;
    }
    void addJson(QJsonObject &var) const override
    {
        Address::addJson(var);
        QJsonArray addresses;
        for (const auto &v : m_addresses)
        {

            if (v.first)
            {
                QJsonObject weightedAddress;
                QJsonObject address;
                v.first->addJson(address);
                weightedAddress.insert("address", address);
                weightedAddress.insert("weight", (int)v.second);
                addresses.append(weightedAddress);
            }
        }
        var.insert("addresses", addresses);
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
