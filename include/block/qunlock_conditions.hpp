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

    Unlock_Condition(quint8 typ );
    static Unlock_Condition * from_Json(const QJsonValue& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    quint8 type_m;


};

class Address_Unlock_Condition : public Unlock_Condition
{
public:
    Address_Unlock_Condition(Address* address_m);
    Address_Unlock_Condition(const QJsonValue& val);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;


private:
Address * address_;

};


class Storage_Deposit_Return_Unlock_Condition : public Unlock_Condition
{
public:
    Storage_Deposit_Return_Unlock_Condition(Address* return_address_m, quint64 return_amount_m);
    Storage_Deposit_Return_Unlock_Condition(const QJsonValue& val);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;


private:
quint64 return_amount_;
Address* return_address_;

};


class Timelock_Unlock_Condition: public Unlock_Condition
{
public:
    Timelock_Unlock_Condition(quint32 unix_time_m);
    Timelock_Unlock_Condition(const QJsonValue& val);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;

private:
quint32 unix_time_;

};


class Expiration_Unlock_Condition:public Unlock_Condition
{
public:
    Expiration_Unlock_Condition(quint32 unix_time_m,Address* return_address_m);
    Expiration_Unlock_Condition(const QJsonValue& val);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;

private:
Address * return_address_;
quint32 unix_time_;

};
};
};
