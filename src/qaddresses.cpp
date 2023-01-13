#include"block/qaddresses.hpp"
namespace qiota{
namespace qblocks{
void Address::serialize(QDataStream &out)const{};
QJsonObject Address::get_Json(void) const{return QJsonObject();};
Address::Address(types typ ):type_m(typ){};
template<class from_type> std::shared_ptr<Address> Address::from_(from_type& val){
    const auto type_=get_type<types>(val);
    switch(type_) {
      case NFT_typ:
        return std::shared_ptr<Address>(new NFT_Address(val));
      case Alias_typ:
        return std::shared_ptr<Address>(new Alias_Address(val));
      case Ed25519_typ:
        return std::shared_ptr<Address>(new Ed25519_Address(val));
        default:
        return nullptr;

    }
}

template std::shared_ptr<Address> Address::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<Address> Address::from_<const QJsonValueRef>(const QJsonValueRef& val);
template std::shared_ptr<Address> Address::from_<QDataStream>(QDataStream & val);


template<class derived_> std::shared_ptr<derived_> Address::to(void)const
{
    return std::shared_ptr<derived_>(new derived_(this));
}
template<> std::shared_ptr<NFT_Address> Address::to(void)const;
template<> std::shared_ptr<Alias_Address> Address::to(void)const;
template<> std::shared_ptr<Ed25519_Address> Address::to(void)const;

NFT_Address::NFT_Address(c_array nft_id_m):Address(NFT_typ),nft_id_(nft_id_m){};
NFT_Address::NFT_Address(const QJsonValue& val):NFT_Address(c_array(val.toObject()["nftId"])){};
NFT_Address::NFT_Address(QDataStream &in):Address(NFT_typ)
{
    nft_id_=c_array(32,0);
    in>>nft_id_;
}
void NFT_Address::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<nft_id_;
}
QJsonObject NFT_Address::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("nftId",nft_id_.toHexString());
    return var;
}

Alias_Address::Alias_Address(c_array alias_id_m):Address(Alias_typ),alias_id_(alias_id_m){};
Alias_Address::Alias_Address(const QJsonValue& val):Alias_Address(c_array(val.toObject()["aliasId"])){};
Alias_Address::Alias_Address(QDataStream &in):Address(Alias_typ)
{
    alias_id_=c_array(32,0);
    in>>alias_id_;
}

void Alias_Address::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<alias_id_;
}
QJsonObject Alias_Address::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("aliasId",alias_id_.toHexString());
    return var;
}

Ed25519_Address::Ed25519_Address(c_array pubkeyhash_m):Address(Ed25519_typ),pubkeyhash_(pubkeyhash_m){};
Ed25519_Address::Ed25519_Address(const QJsonValue& val):Ed25519_Address(c_array(val.toObject()["pubKeyHash"])){};
Ed25519_Address::Ed25519_Address(QDataStream &in):Address(Ed25519_typ)
{
    pubkeyhash_=c_array(32,0);
    in>>pubkeyhash_;
}
void Ed25519_Address::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<pubkeyhash_;
}
QJsonObject Ed25519_Address::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("pubKeyHash",pubkeyhash_.toHexString());
    return var;
}
};
};
