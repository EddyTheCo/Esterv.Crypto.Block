#include"block/qoutputs.hpp"
#include <QCryptographicHash>
namespace qiota{
namespace qblocks{


void Output::serialize(QDataStream &out)const{};
QJsonObject Output::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("amount",QString::number(amount_));
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
};
Output::Output(types typ, quint64 amount_m,
               const std::vector<std::shared_ptr<Unlock_Condition> > &unlock_conditions_m,
               const std::vector<std::shared_ptr<Feature> > &features_m,
               const std::vector<std::shared_ptr<Native_Token> > &native_tokens_m):type_m(typ),
    amount_(amount_m), unlock_conditions_(unlock_conditions_m),features_(features_m),native_tokens_(native_tokens_m)
{
    order_by_type<Unlock_Condition>(unlock_conditions_);
    order_by_type<Feature>(features_);

};
template<class from_type>  std::shared_ptr<Output> Output::from_(from_type& val){
    const auto type_=get_type<types>(val);
    switch(type_) {
    case Basic_typ:
        return std::shared_ptr<Output>(new Basic_Output(val));
    case NFT_typ:
        return std::shared_ptr<Output>(new NFT_Output(val));
    default:
        return nullptr;

    }
}
template std::shared_ptr<Output> Output::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<Output> Output::from_<QDataStream >(QDataStream & val);
template std::shared_ptr<Output> Output::from_<const QJsonValueRef>(const QJsonValueRef& val);
template std::shared_ptr<Output> Output::from_<QJsonValueConstRef const>(QJsonValueConstRef const&);

quint64 Output::min_deposit_of_output(const quint64& wkey,const quint64& wdata,const quint64& v_byte_cost)const
{
    quint64 offset=34*wkey+(32+4+4)*wdata;
    c_array serializedoutput;
    serializedoutput.from_object<Output>(*this);
    quint64 outp=serializedoutput.size()*wdata;
    return (outp+offset)*v_byte_cost;
}

Output::Output(types typ,const QJsonValue& val):Output(typ,val.toObject()["amount"].toString().toULongLong(),
    get_T<Unlock_Condition>(val.toObject()["unlockConditions"].toArray()),
    get_T<Feature>(val.toObject()["features"].toArray()),
    get_T<Native_Token>(val.toObject()["nativeTokens"].toArray())
  ){
};

Basic_Output::Basic_Output(quint64 amount_m, const std::vector<std::shared_ptr<Unlock_Condition>> & unlock_conditions_m,
                           const std::vector<std::shared_ptr<Feature >> & features_m,
                           const std::vector<std::shared_ptr<Native_Token>> & native_tokens_m):Output(types::Basic_typ,amount_m,
                                                                                                      unlock_conditions_m,
                                                                                                      features_m,
                                                                                                      native_tokens_m
                                                                                                      ){};

Basic_Output::Basic_Output(const QJsonValue& val):Output(types::Basic_typ,val)
{
};

Basic_Output::Basic_Output(QDataStream &in):Output(types::Basic_typ)
{
    in>>amount_;
    native_tokens_=deserialize_list<quint8,Native_Token>(in);
    unlock_conditions_=deserialize_list<quint8,Unlock_Condition>(in);
    features_=deserialize_list<quint8,Feature>(in);
}
void Basic_Output::serialize(QDataStream &out)const
{
    out<<type_m<<amount_;
    serialize_native_tokens(out);
    serialize_unlock_conditions(out);
    serialize_features_(out);
}

QJsonObject Basic_Output::get_Json(void) const
{
    return this->Output::get_Json();
}



NFT_Output::NFT_Output(quint64 amount_m, const std::vector<std::shared_ptr<Unlock_Condition>> & unlock_conditions_m,
                       const std::vector<std::shared_ptr<Feature >> & features_m,
                       const std::vector<std::shared_ptr<Native_Token>> & native_tokens_m,
                       std::vector<std::shared_ptr<Feature>> immutable_features_m):
    Output(types::NFT_typ,amount_m,
           unlock_conditions_m,
           features_m,
           native_tokens_m),immutable_features_(immutable_features_m),
    nft_id_(NFT_ID(32,0))
{

order_by_type<Feature>(immutable_features_);
};


NFT_Output::NFT_Output(const QJsonValue& val):Output(types::NFT_typ,val),
    immutable_features_(get_T<Feature>(val.toObject()["immutableFeatures"].toArray())),
    nft_id_(NFT_ID(val.toObject()["nftId"]))
{}

NFT_Output::NFT_Output(QDataStream &in):Output(types::NFT_typ)
{
    in>>amount_;
    native_tokens_=deserialize_list<quint8,Native_Token>(in);
    nft_id_=NFT_ID(32,0);
    in>>nft_id_;
    unlock_conditions_=deserialize_list<quint8,Unlock_Condition>(in);
    features_=deserialize_list<quint8,Feature>(in);
    immutable_features_=deserialize_list<quint8,Feature>(in);
}

QJsonObject NFT_Output::get_Json(void) const
{
    auto var=this->Output::get_Json();
    if(immutable_features_.size())
    {
        QJsonArray fearr;
        for(const auto& v: immutable_features_)fearr.push_back(v->get_Json());
        var.insert("immutableFeatures",fearr);
    }
    var.insert("nftId",nft_id_.toHexString());
    return var;
}
void NFT_Output::serialize(QDataStream &out)const
{

    out<<type_m<<amount_;
    serialize_native_tokens(out);
    out<<nft_id_;

    serialize_unlock_conditions(out);
    serialize_features_(out);
    serialize_immutable_features_(out);
}

};

};
