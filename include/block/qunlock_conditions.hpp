#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include"block/carray.hpp"
#include"block/qaddresses.hpp"
namespace qiota{
namespace qblocks{
class  Unlock_Condition
{
public:
    enum types : quint8 { Address_typ=0, Storage_Deposit_Return_typ=1, Timelock_typ=2, Expiration_typ=3,State_Controller_Address_typ=4,
                          Governor_Address_typ=5, Immutable_Alias_Address_typ=6};
    Unlock_Condition(types typ, const std::shared_ptr<const Address> &address_m=nullptr );
    template<class from_type>static std::shared_ptr<const Unlock_Condition> from_(from_type& val);

    static std::shared_ptr<const Unlock_Condition> Address(const std::shared_ptr<const class Address>& address_m);
    static std::shared_ptr<const Unlock_Condition> Storage_Deposit_Return(const std::shared_ptr<const class Address>& return_address_m,const quint64& return_amount_m);
    static std::shared_ptr<const Unlock_Condition> Timelock(const quint32 &unix_time_m);
    static std::shared_ptr<const Unlock_Condition> Expiration(const quint32& unix_time_m,const std::shared_ptr<const class Address> &return_address_m);
    static std::shared_ptr<const Unlock_Condition> State_Controller_Address(const std::shared_ptr<const class Address>& address_m);
    static std::shared_ptr<const Unlock_Condition> Governor_Address(const std::shared_ptr<const class Address>& address_m);
    static std::shared_ptr<const Unlock_Condition> Immutable_Alias_Address(const std::shared_ptr<const class Address>& address_m);

    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    friend inline bool operator<(const Unlock_Condition& a, const Unlock_Condition& b)
    {
        return a.type_m<b.type_m;
    }
    std::shared_ptr<const class Address>  address(void)const{return address_;}
    types type(void)const{return type_m;}

private:
    const types type_m;
    std::shared_ptr<const class Address> address_;

};

class Address_Unlock_Condition : public Unlock_Condition
{
public:
    Address_Unlock_Condition(const std::shared_ptr<const class Address>& address_m);
    Address_Unlock_Condition(const QJsonValue& val);
    Address_Unlock_Condition(QDataStream &in);

};


class Storage_Deposit_Return_Unlock_Condition : public Unlock_Condition
{
public:
    Storage_Deposit_Return_Unlock_Condition(const std::shared_ptr<const class Address>& return_address_m,const quint64& return_amount_m);
    Storage_Deposit_Return_Unlock_Condition(const QJsonValue& val);
    Storage_Deposit_Return_Unlock_Condition(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;
    quint64 return_amount(void)const{return return_amount_;}

private:
    quint64 return_amount_;

};


class Timelock_Unlock_Condition: public Unlock_Condition
{
public:
    Timelock_Unlock_Condition(const quint32 &unix_time_m);
    Timelock_Unlock_Condition(const QJsonValue& val);
    Timelock_Unlock_Condition(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;
    quint32 unix_time(void)const{return unix_time_;}

private:
    quint32 unix_time_;

};


class Expiration_Unlock_Condition:public Unlock_Condition
{
public:
    Expiration_Unlock_Condition(const quint32& unix_time_m,const std::shared_ptr<const class Address> &return_address_m);
    Expiration_Unlock_Condition(const QJsonValue& val);
    Expiration_Unlock_Condition(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;
    quint32 unix_time(void)const{return unix_time_;}

private:
    quint32 unix_time_;

};

class State_Controller_Address_Unlock_Condition:public Unlock_Condition
{
public:
    State_Controller_Address_Unlock_Condition(const std::shared_ptr<const class Address>& address_m);
    State_Controller_Address_Unlock_Condition(const QJsonValue& val);
    State_Controller_Address_Unlock_Condition(QDataStream &in);

};

class Governor_Address_Unlock_Condition:public Unlock_Condition
{
public:
    Governor_Address_Unlock_Condition(const std::shared_ptr<const class Address>& address_m);
    Governor_Address_Unlock_Condition(const QJsonValue& val);
    Governor_Address_Unlock_Condition(QDataStream &in);
};
class Immutable_Alias_Address_Unlock_Condition:public Unlock_Condition
{
public:
    Immutable_Alias_Address_Unlock_Condition(const std::shared_ptr<const class Address>& address_m);
    Immutable_Alias_Address_Unlock_Condition(const QJsonValue& val);
    Immutable_Alias_Address_Unlock_Condition(QDataStream &in);

};
};
};
