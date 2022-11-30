
#include"block/qsignatures.hpp"
namespace qiota{
namespace qblocks{
void Signature::serialize(QDataStream &out)const{};
QJsonObject Signature::get_Json(void) const{return QJsonObject();};
Signature::Signature(quint8 typ ):type_m(typ){};
Signature * Signature::from_Json(const QJsonValue& val){
    const quint8 type_=val.toObject()["type"].toInt();

    switch(type_) {
      case 0:
        return new Ed25519_Signature(val);
        default:
        return nullptr;

    }
}

Ed25519_Signature::Ed25519_Signature(public_key public_key_m,signature signature_m):Signature(0),public_key_(public_key_m),signature_(signature_m){};
Ed25519_Signature::Ed25519_Signature(const QJsonValue& val):
    Ed25519_Signature(public_key(val.toObject()["publicKey"]),signature(val.toObject()["signature"])){};
void Ed25519_Signature::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<public_key_;
    out<<signature_;
}
QJsonObject Ed25519_Signature::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("publicKey",public_key_.toHexString());
    var.insert("signature",signature_.toHexString());
    return var;
}


};

};
