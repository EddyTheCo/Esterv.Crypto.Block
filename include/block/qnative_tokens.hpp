#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include"block/carray.hpp"
namespace qiota{


namespace qblocks{


class Native_Token
{
public:
    Native_Token(const token_id& token_id_m, const quint256 amount_m);
    Native_Token(const QJsonValue& val);
    Native_Token(QDataStream &in);
    template<class from_type> static std::shared_ptr<Native_Token> from_(from_type& val);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;

private:
    token_id token_id_; //ByteArray[38] Identifier of the native token.
    quint256 amount_;  //Amount of native tokens of the given Token ID. uint256->ByteArray[32]

};

};

};
