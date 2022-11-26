#include"block/qnative_tokens.hpp"
namespace qiota{
namespace qblocks{
Native_Token::Native_Token(){};
Native_Token::Native_Token(const QJsonValue& val):Native_Token(){};
Native_Token * Native_Token::from_Json(const QJsonValue& val){

    return new Native_Token(val);
}
void Native_Token::serialize(QDataStream &out)const
{

}
QJsonObject Native_Token::get_Json(void) const
{
    QJsonObject var;
    return var;
}

};

};
