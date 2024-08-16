
#include "esterv/crypto/block/signatures.hpp"

namespace esterv::crypto::block
{

template <class from_type> std::shared_ptr<const Signature> Signature::from(from_type &val)
{
    const auto type = getType<SignatureType>(val);
    switch (type)
    {
    case SignatureType::Ed25519:
        return std::shared_ptr<const Signature>(new Ed25519Signature(val));
    default:
        return nullptr;
    }
}
template std::shared_ptr<const Signature> Signature::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const Signature> Signature::from<QDataStream>(QDataStream &val);
template std::shared_ptr<const Signature> Signature::from<const QJsonValueRef>(const QJsonValueRef &val);

std::shared_ptr<const Signature> Signature::Ed25519(const c_array &publicKey, const c_array &signature)
{
    return std::shared_ptr<const Signature>(new Ed25519Signature(publicKey, signature));
}

}; // namespace esterv::crypto::block
