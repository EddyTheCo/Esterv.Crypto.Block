#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include<QJsonArray>
#include"block/carray.hpp"
#include"block/qunlock_conditions.hpp"
#include"block/qfeatures.hpp"
#include"block/qnative_tokens.hpp"
namespace qiota{
namespace qblocks{

class Output
{
public:
    Output(quint8 typ ,quint64 amount_m);
    static Output * from_Json(const QJsonValue& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;
    quint8 type_m;
    quint64 amount_;
};

class Basic_Output :public Output
{
public:
    Basic_Output(quint64 amount_m, std::vector<Unlock_Condition *>  unlock_conditions_m,
                 std::vector<Feature *>  features_m,
                 std::vector<Native_Token*>  native_tokens_m);
     Basic_Output(const QJsonValue& val);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;


private:

    std::vector<Unlock_Condition *> unlock_conditions_;
    std::vector<Feature *> features_;
    std::vector<Native_Token*> native_tokens_;

};

};

};
