#pragma once


#include"block/carray.hpp"
#include<QJsonObject>
namespace qiota{
namespace qblocks{

class  Address
{
public:

    Address(c_array& addr_m,quint8 typ );
    static Address * from_Json(const QJsonValue& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    quint8 type_m;
    c_array addr_;

};

class NFT_Address : public Address
{
public:
    NFT_Address(c_array addr_m);
    NFT_Address(const QJsonValue& val);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;


};
class Alias_Address : public Address
{
public:
    Alias_Address(c_array addr_m);
    Alias_Address(const QJsonValue& val);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;


};

class Ed25519_Address : public Address
{
public:
    Ed25519_Address(c_array addr_m);
    Ed25519_Address(const QJsonValue& val);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;

};
};
};
