#include"block/qoutputs.hpp"

namespace qiota{
namespace qblocks{
void Output::serialize(QDataStream &out)const{};
QJsonObject Output::get_Json(void) const{return QJsonObject();};
Output::Output(quint8 typ ,quint64 amount_m):type_m(typ),amount_(amount_m){};
Output * Output::from_Json(const QJsonValue& val){
    const quint8 type_=val.toObject()["type"].toInt();

    switch(type_) {
      case 3:
        return new Basic_Output(val);
    default:
    return nullptr;

    }
}

Basic_Output::Basic_Output(quint64 amount_m, std::vector<Unlock_Condition *>  unlock_conditions_m,
             std::vector<Feature *>  features_m,
             std::vector<Native_Token*>  native_tokens_m):Output(3,amount_m),
    unlock_conditions_(unlock_conditions_m),features_(features_m),native_tokens_(native_tokens_m){};

Basic_Output::Basic_Output(const QJsonValue& val):Basic_Output(
                               val.toObject()["amount"].toString().toULongLong(),
    get_T<Unlock_Condition>(val.toObject()["unlockConditions"].toArray()),
    get_T<Feature>(val.toObject()["features"].toArray()),
    get_T<Native_Token>(val.toObject()["nativeTokens"].toArray())
                                                      ){
};

void Basic_Output::serialize(QDataStream &out)const
{
    out<<type_m<<amount_;
    out<<static_cast<quint8>(native_tokens_.size());
    for(const auto& v: native_tokens_)v->serialize(out);

    out<<static_cast<quint8>(unlock_conditions_.size());
    for(const auto& v: unlock_conditions_)v->serialize(out);
    out<<static_cast<quint8>(features_.size());
    for(const auto& v: features_)v->serialize(out);
}
QJsonObject Basic_Output::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("amount",QString::number(amount_)); //fix this
    if(native_tokens_.size())
    {
         QJsonArray naarr;
         for(const auto& v: native_tokens_)naarr.push_back(v->get_Json());
         var.insert("nativeTokens",naarr);
    }

    QJsonArray unarr;
    for(const auto& v: unlock_conditions_)unarr.push_back(v->get_Json());
    var.insert("unlockConditions",unarr);
    if(features_.size())
    {
         QJsonArray fearr;
         for(const auto& v: features_)fearr.push_back(v->get_Json());
         var.insert("features",fearr);
    }
    return var;
}

};

};
