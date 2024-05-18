
#include "block/qsignatures.hpp"
namespace qiota
{
namespace qblocks
{
void Signature::serialize(QDataStream &out) const {};
QJsonObject Signature::get_Json(void) const
{
    return QJsonObject();
};
Signature::Signature(types typ) : type_m(typ){};
template <class from_type> std::shared_ptr<const Signature> Signature::from_(from_type &val)
{
    const auto type_ = get_type<quint8>(val);
    switch (type_)
    {
    case Ed25519_typ:
        return std::shared_ptr<Signature>(new Ed25519_Signature(val));
    default:
        return nullptr;
    }
}
template std::shared_ptr<const Signature> Signature::from_<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const Signature> Signature::from_<QDataStream>(QDataStream &val);
template std::shared_ptr<const Signature> Signature::from_<const QJsonValueRef>(const QJsonValueRef &val);

std::shared_ptr<const Signature> Signature::Ed25519(const public_key &public_key_m, const signature &signature_m)
{
    return std::shared_ptr<Signature>(new Ed25519_Signature(public_key_m, signature_m));
}

Ed25519_Signature::Ed25519_Signature(const public_key &public_key_m, const signature &signature_m)
    : Signature(Ed25519_typ), public_key_(public_key_m), signature_(signature_m){};
Ed25519_Signature::Ed25519_Signature(const QJsonValue &val)
    : Ed25519_Signature(public_key(val.toObject()["publicKey"]), signature(val.toObject()["signature"])){};
Ed25519_Signature::Ed25519_Signature(QDataStream &in) : Signature(Ed25519_typ)
{
    public_key_ = public_key(32, 0);
    in >> public_key_;
    signature_ = signature(64, 0);
    in >> signature_;
};
void Ed25519_Signature::serialize(QDataStream &out) const
{
    out << type();
    out << public_key_;
    out << signature_;
}
QJsonObject Ed25519_Signature::get_Json(void) const
{
    QJsonObject var;
    var.insert("type", (int)type());
    var.insert("publicKey", public_key_.toHexString());
    var.insert("signature", signature_.toHexString());
    return var;
}

}; // namespace qblocks

}; // namespace qiota
