#include<block/qtoken_scheme.hpp>
namespace qiota{
namespace qblocks{

void Token_Scheme::serialize(QDataStream &out)const{};
QJsonObject Token_Scheme::get_Json(void) const{return QJsonObject();};
Token_Scheme::Token_Scheme(types typ ):type_m(typ){};

template<class from_type> std::shared_ptr<Token_Scheme> Token_Scheme::from_(from_type& val){
    const auto type_=get_type<quint8>(val);

    switch(type_) {

      case Simple_typ:
        return std::shared_ptr<Token_Scheme>(new Simple_Token_Scheme(val));
    default:
    return nullptr;

    }
}
template std::shared_ptr<Token_Scheme> Token_Scheme::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<Token_Scheme> Token_Scheme::from_<QDataStream >(QDataStream & val);
template std::shared_ptr<Token_Scheme> Token_Scheme::from_<const QJsonValueRef>(const QJsonValueRef& val);
template std::shared_ptr<Token_Scheme> Token_Scheme::from_<QJsonValueConstRef const>(QJsonValueConstRef const&);


Simple_Token_Scheme::Simple_Token_Scheme(quint256  minted_tokens_m,quint256 melted_tokens_m,quint256 maximum_supply_m)
    :Token_Scheme(Simple_typ),minted_tokens_(minted_tokens_m),melted_tokens_(melted_tokens_m),maximum_supply_(maximum_supply_m)
{};
Simple_Token_Scheme::Simple_Token_Scheme(const QJsonValue& val):
    Simple_Token_Scheme(quint256(val.toObject()["mintedTokens"].toString()),
    quint256(val.toObject()["meltedTokens"].toString()),
    quint256(val.toObject()["maximumSupply"].toString())){};

Simple_Token_Scheme::Simple_Token_Scheme(QDataStream &in):Token_Scheme(Simple_typ)
{
    in>>minted_tokens_;
    in>>melted_tokens_;
    in>>maximum_supply_;
};
void Simple_Token_Scheme::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<minted_tokens_;
    out<<melted_tokens_;
    out<<maximum_supply_;
}
QJsonObject Simple_Token_Scheme::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("mintedTokens",minted_tokens_.toHex());
    var.insert("meltedTokens",melted_tokens_.toHex());
    var.insert("maximumSupply",maximum_supply_.toHex());
    return var;
}
};
};
