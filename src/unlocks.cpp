
#include "esterv/crypto/block/unlocks.hpp"
namespace esterv::crypto::block
{

template <typename from_type> std::shared_ptr<const Unlock> Unlock::from(from_type &val)
{
    const auto type = getType<UnlockType>(val);

    switch (type)
    {
    case UnlockType::Signature:
        return std::shared_ptr<const Unlock>(new SignatureUnlock(val));
    case UnlockType::Reference:
        return std::shared_ptr<const Unlock>(new ReferenceUnlock(val));
    case UnlockType::Account:
        return std::shared_ptr<const Unlock>(new AccountUnlock(val));
    case UnlockType::Anchor:
        return std::shared_ptr<const Unlock>(new AnchorUnlock(val));
    case UnlockType::NFT:
        return std::shared_ptr<const Unlock>(new NFTUnlock(val));
    case UnlockType::Empty:
        return std::shared_ptr<const Unlock>(new EmptyUnlock());
    default:
        return nullptr;
    }
}

template std::shared_ptr<const Unlock> Unlock::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const Unlock> Unlock::from<QDataStream>(QDataStream &val);
template std::shared_ptr<const Unlock> Unlock::from<const QJsonValueRef>(const QJsonValueRef &val);
template std::shared_ptr<const Unlock> Unlock::from<QJsonValueConstRef const>(QJsonValueConstRef const &);

std::shared_ptr<const Unlock> Unlock::Signature(const std::shared_ptr<const class Signature> &signature)
{
    return std::shared_ptr<const Unlock>(new SignatureUnlock(signature));
}

std::shared_ptr<const Unlock> Unlock::Reference(const quint16 &reference)
{
    return std::shared_ptr<const Unlock>(new ReferenceUnlock(reference));
}

std::shared_ptr<const Unlock> Unlock::Account(const quint16 &reference)
{
    return std::shared_ptr<const Unlock>(new AccountUnlock(reference));
}

std::shared_ptr<const Unlock> Unlock::Anchor(const quint16 &reference)
{
    return std::shared_ptr<const Unlock>(new AnchorUnlock(reference));
}

std::shared_ptr<const Unlock> Unlock::NFT(const quint16 &reference)
{
    return std::shared_ptr<const Unlock>(new NFTUnlock(reference));
}
std::shared_ptr<const Unlock> Unlock::Empty()
{
    return std::shared_ptr<const Unlock>(new EmptyUnlock());
}

}; // namespace esterv::crypto::block
