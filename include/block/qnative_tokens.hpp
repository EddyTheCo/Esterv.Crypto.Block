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
    Native_Token(const Token_ID& token_id_m, const quint256& amount_m);
    Native_Token(const QJsonValue& val);
    Native_Token(QDataStream &in);
    template<class from_type> static std::shared_ptr<const Native_Token> from_(from_type& val);
    template<class deriv, class... Types> static std::shared_ptr<const Native_Token> get(Types... args)
    {
        return std::shared_ptr<Native_Token>(new deriv(args...));
    }
    void serialize(QDataStream &out)const;

    friend inline bool operator<(const Native_Token& a, const Native_Token& b)
    {
        return a.token_id_<b.token_id_;
    }
    QJsonObject get_Json(void) const;
    const Token_ID token_id(void)const{return token_id_;}
    quint256 amount(void)const{return amount_;}

private:
    Token_ID token_id_; //ByteArray[38] Identifier of the native token.
    quint256 amount_;  //Amount of native tokens of the given Token ID. uint256->ByteArray[32]

};

};

};
