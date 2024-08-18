#include "esterv/crypto/block/addresses.hpp"

namespace esterv::crypto::block
{

const QHash<AddressType, QString> HashAddress::JsonStr{{AddressType::Anchor, "anchorId"},
                                                       {AddressType::NFT, "nftId"},
                                                       {AddressType::Account, "accountId"},
                                                       {AddressType::Ed25519, "pubKeyHash"}};

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
    case AddressType::Multi:
        return std::shared_ptr<const Address>{new MultiAddress(val)};
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

std::shared_ptr<const Address> Address::Multi(
    const std::initializer_list<std::pair<std::shared_ptr<const Address>, quint8>> &weightedAddresses,
    const quint16 &threshold)
{
    return std::shared_ptr<const Address>{new MultiAddress(weightedAddresses, threshold)};
}

} // namespace qiota::qblocks
