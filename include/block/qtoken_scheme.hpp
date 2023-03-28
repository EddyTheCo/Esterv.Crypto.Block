#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include"block/carray.hpp"
namespace qiota{
namespace qblocks{
class  Token_Scheme
{
public:
    enum types : quint8 { Simple_typ=0};
    Token_Scheme(types typ);
    template<class from_type> static std::shared_ptr<Token_Scheme> from_(from_type& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    const types type_m;

};


class Simple_Token_Scheme : public Token_Scheme
{
public:
    Simple_Token_Scheme(quint256  minted_tokens_m,quint256 melted_tokens_m,quint256 maximum_supply_m);
    Simple_Token_Scheme(const QJsonValue& val);
    Simple_Token_Scheme(QDataStream &in);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;


private:
quint256 minted_tokens_,melted_tokens_,maximum_supply_;


};

};

};
