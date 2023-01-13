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
    enum types : quint8 { Address_typ, Storage_Deposit_Return_typ, Timelock_typ, Expiration_typ };
    Unlock_Condition(types typ );
    template<class from_type>static std::shared_ptr<Unlock_Condition> from_(from_type& val);
    template<class derived_> std::shared_ptr<derived_> to(void)const;

    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    const types type_m;


};

class Address_Unlock_Condition : public Unlock_Condition
{
public:
    Address_Unlock_Condition(const std::shared_ptr<Address>& address_m);
    Address_Unlock_Condition(const QJsonValue& val);
    Address_Unlock_Condition(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;
    std::shared_ptr<Address>  address(void)const{return address_;}  


private:
std::shared_ptr<Address> address_;

};


class Storage_Deposit_Return_Unlock_Condition : public Unlock_Condition
{
public:
    Storage_Deposit_Return_Unlock_Condition(const std::shared_ptr<Address> &return_address_m, quint64 return_amount_m);
    Storage_Deposit_Return_Unlock_Condition(const QJsonValue& val);
    Storage_Deposit_Return_Unlock_Condition(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;
    quint64 return_amount(void)const{return return_amount_;}
    std::shared_ptr<Address> return_address(void)const{return return_address_;}

private:
quint64 return_amount_;
std::shared_ptr<Address> return_address_;

};


class Timelock_Unlock_Condition: public Unlock_Condition
{
public:
    Timelock_Unlock_Condition(quint32 unix_time_m);
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
    Expiration_Unlock_Condition(quint32 unix_time_m,const std::shared_ptr<Address> &return_address_m);
    Expiration_Unlock_Condition(const QJsonValue& val);
    Expiration_Unlock_Condition(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;
    quint32 unix_time(void)const{return unix_time_;}
    std::shared_ptr<Address> return_address()const{return return_address_;}
private:
std::shared_ptr<Address> return_address_;
quint32 unix_time_;

};
};
};
