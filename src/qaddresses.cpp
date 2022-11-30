#include"block/qaddresses.hpp"
namespace qiota{
namespace qblocks{
void Address::serialize(QDataStream &out)const{};
QJsonObject Address::get_Json(void) const{return QJsonObject();};
Address::Address(c_array& addr_m,quint8 typ ):type_m(typ),addr_(addr_m){};
Address * Address::from_Json(const QJsonValue& val){
    const quint8 type_=val.toObject()["type"].toInt();

    switch(type_) {
      case 16:
        return new NFT_Address(val);
      case 8:
        return new Alias_Address(val);
      case 0:
        return new Ed25519_Address(val);
        default:
        return nullptr;

    }
}

NFT_Address::NFT_Address(c_array addr_m):Address(addr_m,16){};
NFT_Address::NFT_Address(const QJsonValue& val):NFT_Address(c_array(val.toObject()["nftId"])){};
void NFT_Address::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<addr_;
}
QJsonObject NFT_Address::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("nftId",addr_.toHexString());
    return var;
}

Alias_Address::Alias_Address(c_array addr_m):Address(addr_m,8){};
Alias_Address::Alias_Address(const QJsonValue& val):Alias_Address(c_array(val.toObject()["aliasId"])){};

void Alias_Address::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<addr_;
}
QJsonObject Alias_Address::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("aliasId",addr_.toHexString());
    return var;
}

Ed25519_Address::Ed25519_Address(c_array addr_m):Address(addr_m,0){};
Ed25519_Address::Ed25519_Address(const QJsonValue& val):Ed25519_Address(c_array(val.toObject()["pubKeyHash"])){};
void Ed25519_Address::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<addr_;
}
QJsonObject Ed25519_Address::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("pubKeyHash",addr_.toHexString());
    return var;
}
};
};
