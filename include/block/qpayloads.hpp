#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include<QJsonArray>
#include"block/carray.hpp"
#include"block/qunlocks.hpp"
namespace qiota{
namespace qblocks{
class Essence;
class Payload
{
public:
    Payload(quint32 type_);
    static Payload * from_Json(const QJsonValue& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;
quint32 type_m;

};
class Tagged_Data_Payload : public Payload
{
public:
    Tagged_Data_Payload(const tagF tag_m,const dataF data_m);
    Tagged_Data_Payload(const QJsonValue& val);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;

private:
    tagF tag_;
    dataF data_;

};


class Transaction_Payload: public Payload
{
public:


    Transaction_Payload(Essence* essence_m,std::vector<Unlock*> unlocks_m);
    Transaction_Payload(const QJsonValue& val);

    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;

private:
    Essence* essence_;
    std::vector<Unlock*> unlocks_;
    quint32 type_m;
};
};
};
