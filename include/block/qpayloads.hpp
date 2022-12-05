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
    template<class from_type> static std::shared_ptr<Payload> from_(from_type& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;
quint32 type_m;

};
class Tagged_Data_Payload : public Payload
{
public:
    Tagged_Data_Payload(const tagF tag_m,const dataF data_m);
    Tagged_Data_Payload(const QJsonValue& val);
    Tagged_Data_Payload(QDataStream &in);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;

private:
    tagF tag_;
    dataF data_;

};


class Transaction_Payload: public Payload
{
public:


    Transaction_Payload(const std::shared_ptr<Essence> &essence_m,const std::vector<std::shared_ptr<Unlock>>& unlocks_m);
    Transaction_Payload(const QJsonValue& val);
    Transaction_Payload(QDataStream &in);

    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;

private:
   std::shared_ptr<Essence> essence_;
   std::vector<std::shared_ptr<Unlock>> unlocks_;
};
};
};
