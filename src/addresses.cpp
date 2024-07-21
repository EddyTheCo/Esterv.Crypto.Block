#include "esterv/crypto/block/addresses.hpp"

namespace esterv::crypto::block
{

const QHash<AddressType, QString> Address::JsonStr{{AddressType::Anchor, "anchorId"},
                                                   {AddressType::NFT, "nftId"},
                                                   {AddressType::Account, "accountId"},
                                                   {AddressType::Ed25519, "pubKeyHash"}};

Address::Address(const AddressType typ, const c_array addrhash) : C_Base<AddressType>{typ}, m_addrHash{addrhash}
{
}

Address::Address(const AddressType typ, QDataStream &in) : C_Base<AddressType>(typ)
{
    m_addrHash = c_array(ByteSizes::hash, 0);
    in >> m_addrHash;
}

Address::Address(const AddressType typ, const QJsonValue &val)
    : Address{typ, c_array{val.toObject()[JsonStr.value(typ)]}}
{
}

template <class fromType> std::shared_ptr<const Address> Address::from(fromType &val)
{
    const auto type = getType<AddressType>(val);
    switch (type)
    {
    case AddressType::NFT:
        return std::shared_ptr<const Address>{new NFTAddress(val)};
    case AddressType::Account:
        return std::shared_ptr<const Address>{new AccountAddress(val)};
    case AddressType::Anchor:
        return std::shared_ptr<const Address>{new AnchorAddress(val)};
    case AddressType::Ed25519:
        return std::shared_ptr<const Address>{new Ed25519Address(val)};
    default:
        return nullptr;
    }
}

template std::shared_ptr<const Address> Address::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const Address> Address::from<QDataStream>(QDataStream &val);

std::shared_ptr<const Address> Address::NFT(const c_array addrhash)
{
    return std::shared_ptr<const Address>{new NFTAddress(addrhash)};
}

std::shared_ptr<const Address> Address::Account(const c_array addrhash)
{
    return std::shared_ptr<const Address>{new AccountAddress(addrhash)};
}

std::shared_ptr<const Address> Address::Ed25519(const c_array addrhash)
{
    return std::shared_ptr<const Address>{new Ed25519Address(addrhash)};
}

std::shared_ptr<const Address> Address::Anchor(const c_array addrhash)
{
    return std::shared_ptr<const Address>{new AnchorAddress(addrhash)};
}

} // namespace qiota::qblocks
