
#include"block/qunlocks.hpp"
namespace qiota{
namespace qblocks{
void Unlock::serialize(QDataStream &out)const{};
QJsonObject Unlock::get_Json(void) const{return QJsonObject();};
Unlock::Unlock(types typ ):type_m(typ){};

template<typename from_type> std::shared_ptr<const Unlock> Unlock::from_(from_type& val){
    const auto type_=get_type<quint8>(val);

    switch(type_) {
    case types::Signature_typ:
        return std::shared_ptr<Unlock>(new Signature_Unlock(val));
    case types::Reference_typ:
        return std::shared_ptr<Unlock>(new Reference_Unlock(val));
    case types::Alias_typ:
        return std::shared_ptr<Unlock>(new Alias_Unlock(val));
    case types::NFT_typ:
        return std::shared_ptr<Unlock>(new NFT_Unlock(val));
    default:
        return nullptr;
    }
}


template std::shared_ptr<const Unlock> Unlock::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<const Unlock> Unlock::from_<QDataStream>(QDataStream & val);
template std::shared_ptr<const Unlock> Unlock::from_<const QJsonValueRef>(const QJsonValueRef& val);
template std::shared_ptr<const Unlock> Unlock::from_<QJsonValueConstRef const>(QJsonValueConstRef const&);


Signature_Unlock::Signature_Unlock(const std::shared_ptr<const Signature> &signature_m):Unlock(Signature_typ),signature_(signature_m){};
Signature_Unlock::Signature_Unlock(const QJsonValue& val):Signature_Unlock(Signature::from_<const QJsonValue>(val.toObject()["signature"])){};
Signature_Unlock::Signature_Unlock(QDataStream &in):Unlock(types::Signature_typ),signature_(Signature::from_<QDataStream>(in)){};
void Signature_Unlock::serialize(QDataStream &out)const
{
    out<<type();
    signature_->serialize(out);
}
QJsonObject Signature_Unlock::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type());
    var.insert("signature",signature_->get_Json());
    return var;
}

Reference_Unlock::Reference_Unlock(quint16 reference_m):Unlock(Reference_typ),reference_(reference_m){};
Reference_Unlock::Reference_Unlock(const QJsonValue& val):Reference_Unlock(val.toObject()["reference"].toInt()){};
Reference_Unlock::Reference_Unlock(QDataStream &in):Unlock(Reference_typ)
{
    in>>reference_;
}
void Reference_Unlock::serialize(QDataStream &out)const
{
    out<<type();
    out<<reference_;
}
QJsonObject Reference_Unlock::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type());
    var.insert("reference",(int)reference_);
    return var;
}

Alias_Unlock::Alias_Unlock(quint16 alias_reference_unlock_index_m):Unlock(Alias_typ),alias_reference_unlock_index_(alias_reference_unlock_index_m){};
Alias_Unlock::Alias_Unlock(const QJsonValue& val):Alias_Unlock(val.toObject()["reference"].toInt()){};
Alias_Unlock::Alias_Unlock(QDataStream &in):Unlock(Alias_typ)
{
    in>>alias_reference_unlock_index_;
}
void Alias_Unlock::serialize(QDataStream &out)const
{
    out<<type();
    out<<alias_reference_unlock_index_;
}
QJsonObject Alias_Unlock::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type());
    var.insert("reference",(int)alias_reference_unlock_index_);
    return var;
}


NFT_Unlock::NFT_Unlock(quint16 nft_reference_unlock_index_m):Unlock(NFT_typ),nft_reference_unlock_index_(nft_reference_unlock_index_m){};
NFT_Unlock::NFT_Unlock(const QJsonValue& val):NFT_Unlock(val.toObject()["reference"].toInt()){};
NFT_Unlock::NFT_Unlock(QDataStream &in):Unlock(NFT_typ)
{
    in>>nft_reference_unlock_index_;
}
void NFT_Unlock::serialize(QDataStream &out)const
{
    out<<type();
    out<<nft_reference_unlock_index_;
}
QJsonObject NFT_Unlock::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type());
    var.insert("reference",(int)nft_reference_unlock_index_);
    return var;
}
};
};
