#include"block/qoutputs.hpp"

namespace qiota{
namespace qblocks{
void Output::serialize(QDataStream &out)const{};
QJsonObject Output::get_Json(void) const{return QJsonObject();};
Output::Output(quint8 typ):type_m(typ){};
template<class from_type>  std::shared_ptr<Output> Output::from_(from_type& val){
    const auto type_=get_type<quint8>(val);
    switch(type_) {
      case 3:
        return std::shared_ptr<Output>(new Basic_Output(val));
    default:
    return nullptr;

    }
}
template std::shared_ptr<Output> Output::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<Output> Output::from_<QDataStream >(QDataStream & val);
template std::shared_ptr<Output> Output::from_<const QJsonValueRef>(const QJsonValueRef& val);

Basic_Output::Basic_Output(quint64 amount_m, const std::vector<std::shared_ptr<Unlock_Condition>> & unlock_conditions_m,
             const std::vector<std::shared_ptr<Feature >> & features_m,
             const std::vector<std::shared_ptr<Native_Token>> & native_tokens_m):Output(3),amount_(amount_m),
    unlock_conditions_(unlock_conditions_m),features_(features_m),native_tokens_(native_tokens_m){};

Basic_Output::Basic_Output(const QJsonValue& val):Basic_Output(
                               val.toObject()["amount"].toString().toULongLong(),
    get_T<Unlock_Condition>(val.toObject()["unlockConditions"].toArray()),
    get_T<Feature>(val.toObject()["features"].toArray()),
    get_T<Native_Token>(val.toObject()["nativeTokens"].toArray())
                                                      ){
};
Basic_Output::Basic_Output(QDataStream &in):Output(3)
{
    in>>amount_;
    quint8  native_tokens_count;
    in>>native_tokens_count;
    for(auto i=0;i<native_tokens_count;i++)
    {
        native_tokens_.push_back(Native_Token::from_<QDataStream>(in));
    }
    quint8 unlock_conditions_count;
    in>>unlock_conditions_count;
    for(auto i=0;i<unlock_conditions_count;i++)
    {
        unlock_conditions_.push_back(Unlock_Condition::from_<QDataStream>(in));
    }
    quint8 features_count;
    in>>features_count;
    for(auto i=0;i<features_count;i++)
    {
        features_.push_back(Feature::from_<QDataStream>(in));
    }
}
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
quint64 Basic_Output::min_deposit_of_output(const quint64& wkey,const quint64& wdata,const quint64& v_byte_cost)const
{
    quint64 offset=34*wkey+(32+4+4)*wdata;
    c_array serializedoutput;
    serializedoutput.from_object<Output>(*this);
    quint64 outp=serializedoutput.size()*wdata;
    return (outp+offset)*v_byte_cost;
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
