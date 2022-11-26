
#include"block/qunlocks.hpp"
namespace qiota{
namespace qblocks{
void Unlock::serialize(QDataStream &out)const{};
QJsonObject Unlock::get_Json(void) const{return QJsonObject();};
Unlock::Unlock(quint8 typ ):type_m(typ){};
Unlock * Unlock::from_Json(const QJsonValue& val){
    const quint8 type_=val.toObject()["type"].toInt();

    switch(type_) {
    case 1:
        return new Reference_Unlock(val);
    case 2:
        return new Alias_Unlock(val);
    case 3:
        return new NFT_Unlock(val);
    default:
        return new Signature_Unlock(val);

    }
}

Signature_Unlock::Signature_Unlock(Signature * signature_m):Unlock(0),signature_(signature_m){};
Signature_Unlock::Signature_Unlock(const QJsonValue& val):Signature_Unlock(Signature::from_Json(val.toObject()["signature"])){};
void Signature_Unlock::serialize(QDataStream &out)const
{
    out<<type_m;
    signature_->serialize(out);
}
QJsonObject Signature_Unlock::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",QString::number(type_m));
    var.insert("signature",signature_->get_Json());
    return var;
}

Reference_Unlock::Reference_Unlock(quint16 reference_m):Unlock(1),reference_(reference_m){};
Reference_Unlock::Reference_Unlock(const QJsonValue& val):Reference_Unlock(val.toObject()["reference"].toInt()){};
void Reference_Unlock::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<reference_;
}
QJsonObject Reference_Unlock::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",QString::number(type_m));
    var.insert("reference",QString::number(reference_));
    return var;
}

Alias_Unlock::Alias_Unlock(quint16 alias_reference_unlock_index_m):Unlock(2),alias_reference_unlock_index_(alias_reference_unlock_index_m){};
Alias_Unlock::Alias_Unlock(const QJsonValue& val):Alias_Unlock(val.toObject()["reference"].toInt()){};
void Alias_Unlock::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<alias_reference_unlock_index_;
}
QJsonObject Alias_Unlock::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",QString::number(type_m));
    var.insert("reference",QString::number(alias_reference_unlock_index_));
    return var;
}


NFT_Unlock::NFT_Unlock(quint16 nft_reference_unlock_index_m):Unlock(3),nft_reference_unlock_index_(nft_reference_unlock_index_m){};
NFT_Unlock::NFT_Unlock(const QJsonValue& val):NFT_Unlock(val.toObject()["reference"].toInt()){};
void NFT_Unlock::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<nft_reference_unlock_index_;
}
QJsonObject NFT_Unlock::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",QString::number(type_m));
    var.insert("reference",QString::number(nft_reference_unlock_index_));
    return var;
}
};
};
