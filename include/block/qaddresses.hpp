#pragma once


#include"block/carray.hpp"
#include<QJsonObject>
namespace qiota{
namespace qblocks{

class  Address
{
public:
    enum types : quint8 { NFT_typ=16, Alias_typ=8, Ed25519_typ=0};

    Address(types typ );
    template<class from_type> static std::shared_ptr<Address> from_(from_type& val);


    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    const types type_m;

};

class NFT_Address : public Address
{
public:
    NFT_Address(c_array addr_m);
    NFT_Address(const QJsonValue& val);
    NFT_Address(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;

private:
    c_array nft_id_;

};
class Alias_Address : public Address
{
public:
    Alias_Address(c_array addr_m);
    Alias_Address(const QJsonValue& val);
    Alias_Address(QDataStream &in);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;

private:
    c_array alias_id_;

};

class Ed25519_Address : public Address
{
public:
    Ed25519_Address(c_array addr_m);
    Ed25519_Address(const QJsonValue& val);
    Ed25519_Address(QDataStream &in);
    void serialize(QDataStream &out)const;
    c_array pubkeyhash(void)const{return pubkeyhash_;}
    QJsonObject get_Json(void) const;
private:
    c_array pubkeyhash_;

};
};
};
