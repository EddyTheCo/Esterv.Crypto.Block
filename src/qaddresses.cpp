#include"block/qaddresses.hpp"
namespace qiota{
namespace qblocks{
void Address::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<addrhash_;
};
QJsonObject Address::get_Json(void) const{return QJsonObject();};
Address::Address(types typ, c_array addrhash_m ):type_m(typ),addrhash_(addrhash_m){};
Address::Address(types typ, QDataStream &in):type_m(typ)
{
    addrhash_=c_array(32,0);
    in>>addrhash_;
}
template<class from_type> std::shared_ptr<const Address> Address::from_(from_type& val){
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



template std::shared_ptr<const Address> Address::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<const Address> Address::from_<const QJsonValueRef>(const QJsonValueRef& val);
template std::shared_ptr<const Address> Address::from_<QDataStream>(QDataStream & val);

std::shared_ptr<const Address> Address::from_(QByteArray& val)
{
    auto buffer=QDataStream(&val,QIODevice::ReadOnly);
    buffer.setByteOrder(QDataStream::LittleEndian);
    return qblocks::Address::from_<QDataStream>(buffer);
}

NFT_Address::NFT_Address(const c_array &nft_id_m):Address(NFT_typ,nft_id_m){};
NFT_Address::NFT_Address(const QJsonValue& val):NFT_Address(NFT_ID(val.toObject()["nftId"])){};
NFT_Address::NFT_Address(QDataStream &in):Address(NFT_typ,in){};

QJsonObject NFT_Address::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type());
    var.insert("nftId",addrhash().toHexString());
    return var;
}

Alias_Address::Alias_Address(const c_array &alias_id_m):Address(Alias_typ,alias_id_m){};
Alias_Address::Alias_Address(const QJsonValue& val):Alias_Address(c_array(val.toObject()["aliasId"])){};
Alias_Address::Alias_Address(QDataStream &in):Address(Alias_typ,in){};


QJsonObject Alias_Address::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type());
    var.insert("aliasId",addrhash().toHexString());
    return var;
}

Ed25519_Address::Ed25519_Address(const c_array& pubkeyhash_m):Address(Ed25519_typ,pubkeyhash_m){};
Ed25519_Address::Ed25519_Address(const QJsonValue& val):Ed25519_Address(c_array(val.toObject()["pubKeyHash"])){};
Ed25519_Address::Ed25519_Address(QDataStream &in):Address(Ed25519_typ,in){};

QJsonObject Ed25519_Address::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type());
    var.insert("pubKeyHash",addrhash().toHexString());
    return var;
}
};
};
