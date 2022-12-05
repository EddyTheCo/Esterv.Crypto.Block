#include"block/qnative_tokens.hpp"
namespace qiota{
namespace qblocks{
Native_Token::Native_Token(const token_id& token_id_m,const c_array amount_m):token_id_(token_id_m),
amount_(amount_m){};
Native_Token::Native_Token(const QJsonValue& val):token_id_(token_id(val.toObject()["tokenId"])),
    amount_(c_array(val.toObject()["amount"])){};

template<class from_type> std::shared_ptr<Native_Token> Native_Token::from_(from_type& val){
    return std::shared_ptr<Native_Token>(new Native_Token(val));
}

template std::shared_ptr<Native_Token> Native_Token::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<Native_Token> Native_Token::from_<QDataStream >(QDataStream & val);
template std::shared_ptr<Native_Token> Native_Token::from_<const QJsonValueRef>(const QJsonValueRef& val);
Native_Token::Native_Token(QDataStream &in)
{
    in>>token_id_;
    in>>amount_;
};


void Native_Token::serialize(QDataStream &out)const
{
    out<<token_id_;
    out<<amount_;
}
QJsonObject Native_Token::get_Json(void) const
{
    QJsonObject var;
    var.insert("tokenId",token_id_.toHexString());
    var.insert("amount_",token_id_.toHexString());
    return var;
}

};

};
