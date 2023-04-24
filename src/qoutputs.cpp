#include"block/qoutputs.hpp"
namespace qiota{
namespace qblocks{

const  QHash<Output::types,QString > Output::typesstr=
{{Basic_typ,"basic"},{NFT_typ,"nft"},{Foundry_typ,"foundry"},{Alias_typ,"alias"}};


void Output::serialize(QDataStream &out)const{};
void Output::set_id(const c_array& id){};
void Output::consume(void)
{
    features_.clear();
    native_tokens_.clear();
    unlock_conditions_.clear();
};
void Foundry_Output::consume(void)
{
    features_.clear();
    native_tokens_.clear();
};
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
    if(immutable_features_.size())
    {
        QJsonArray fearr;
        for(const auto& v: immutable_features_)fearr.push_back(v->get_Json());
        var.insert("immutableFeatures",fearr);
    }
    return var;
};
Output::Output(types typ, const quint64 &amount_m,
               const pvector<const Unlock_Condition> &unlock_conditions_m,
               const pvector<const Feature> &features_m,
               const pvector<const Native_Token> &native_tokens_m,
               const pvector<const Feature> &immutable_features_m):type_m(typ),
    amount_(amount_m), unlock_conditions_(unlock_conditions_m),features_(features_m),native_tokens_(native_tokens_m),immutable_features_(immutable_features_m)
{
    order_list<const Unlock_Condition>(unlock_conditions_);
    order_list<const Feature>(features_);
    order_list<const Feature>(immutable_features_);
    order_list<const Native_Token>(native_tokens_);

};
template<class from_type>  std::shared_ptr<Output> Output::from_(from_type& val){
    const auto type_=get_type<types>(val);
    switch(type_) {
    case Basic_typ:
        return std::shared_ptr<Output>(new Basic_Output(val));
    case NFT_typ:
        return std::shared_ptr<Output>(new NFT_Output(val));
    case Foundry_typ:
        return std::shared_ptr<Output>(new Foundry_Output(val));
    case Alias_typ:
        return std::shared_ptr<Output>(new Alias_Output(val));
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
    get_T<Native_Token>(val.toObject()["nativeTokens"].toArray()),
    get_T<Feature>(val.toObject()["immutableFeatures"].toArray())
  ){};


std::shared_ptr<Output> Output::Basic(const quint64 &amount_m,
                                      const pvector<const Unlock_Condition> &unlock_conditions_m,
                                      const pvector<const Native_Token> &native_tokens_m,
                                      const pvector<const Feature> &features_m)
{
    return std::shared_ptr<Output>(new Basic_Output(amount_m,unlock_conditions_m,native_tokens_m,features_m));
}
Basic_Output::Basic_Output(const quint64& amount_m, const pvector<const Unlock_Condition>  &unlock_conditions_m,
                           const pvector<const Native_Token>  &native_tokens_m,
                           const pvector<const Feature>  &features_m):Output(types::Basic_typ,amount_m,
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
    out<<type()<<amount_;
    serialize_list<quint8>(out,native_tokens_);
    serialize_list<quint8>(out,unlock_conditions_);
    serialize_list<quint8>(out,features_);

}

QJsonObject Basic_Output::get_Json(void) const
{
    return this->Output::get_Json();
}

std::shared_ptr<Output> Output::NFT(const quint64 &amount_m, const pvector<const Unlock_Condition> &unlock_conditions_m,
                                    const pvector<const Native_Token> &native_tokens_m,
                                    const pvector<const Feature> &immutable_features_m,
                                    const pvector<const Feature> &features_m)
{
    return std::shared_ptr<Output>(new NFT_Output(amount_m,unlock_conditions_m,native_tokens_m,immutable_features_m,features_m));
}


NFT_Output::NFT_Output(const quint64& amount_m, const pvector<const Unlock_Condition> &unlock_conditions_m,
                       const pvector<const Native_Token> &native_tokens_m,
                       const pvector<const Feature>& immutable_features_m,
                       const pvector<const Feature> &features_m):
    Output(types::NFT_typ,amount_m,
           unlock_conditions_m,
           features_m,
           native_tokens_m,
           immutable_features_m),
    nft_id_(NFT_ID(32,0))
{};


NFT_Output::NFT_Output(const QJsonValue& val):Output(types::NFT_typ,val),
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

    var.insert("nftId",nft_id_.toHexString());
    return var;
}
void NFT_Output::serialize(QDataStream &out)const
{

    out<<type()<<amount_;
    serialize_list<quint8>(out,native_tokens_);
    out<<nft_id_;
    serialize_list<quint8>(out,unlock_conditions_);
    serialize_list<quint8>(out,features_);
    serialize_list<quint8>(out,immutable_features_);
}

std::shared_ptr<Output> Output::Foundry(const quint64 &amount_m, const pvector<const Unlock_Condition> &unlock_conditions_m,
                                        const std::shared_ptr<Token_Scheme> &token_scheme_m, const quint32& serial_number_m,
                                        const pvector<const Native_Token> &native_tokens_m,
                                        const pvector<const Feature> &immutable_features_m,
                                        const pvector<const Feature> &features_m)
{
    return std::shared_ptr<Output>(new Foundry_Output(amount_m,unlock_conditions_m,token_scheme_m,serial_number_m,native_tokens_m,immutable_features_m,features_m));
}

Foundry_Output::Foundry_Output(const quint64 &amount_m, const pvector<const Unlock_Condition> &unlock_conditions_m,
                               const std::shared_ptr<Token_Scheme> &token_scheme_m, const quint32& serial_number_m,
                               const pvector<const Native_Token> &native_tokens_m,
                               const pvector<const Feature> &immutable_features_m,
                               const pvector<const Feature> &features_m):
    Output(types::Foundry_typ,amount_m,
           unlock_conditions_m,
           features_m,
           native_tokens_m,
           immutable_features_m),
    token_scheme_(token_scheme_m),serial_number_(serial_number_m)
{};
Foundry_Output::Foundry_Output(const QJsonValue& val):Output(types::Foundry_typ,val),
    token_scheme_(Token_Scheme::from_<const QJsonValue>(val.toObject()["tokenScheme"])),serial_number_(val.toObject()["serialNumber"].toInteger())
{}

Foundry_Output::Foundry_Output(QDataStream &in):Output(types::Foundry_typ)
{
    in>>amount_;
    native_tokens_=deserialize_list<quint8,Native_Token>(in);
    in>>serial_number_;
    token_scheme_=Token_Scheme::from_<QDataStream>(in);
    unlock_conditions_=deserialize_list<quint8,Unlock_Condition>(in);
    features_=deserialize_list<quint8,Feature>(in);
    immutable_features_=deserialize_list<quint8,Feature>(in);
}

QJsonObject Foundry_Output::get_Json(void) const
{
    auto var=this->Output::get_Json();
    var.insert("serialNumber",(int)serial_number_);
    var.insert("tokenScheme",token_scheme_->get_Json());
    return var;
}
void Foundry_Output::serialize(QDataStream &out)const
{

    out<<type()<<amount_;
    serialize_list<quint8>(out,native_tokens_);
    out<<serial_number_;
    token_scheme_->serialize(out);
    serialize_list<quint8>(out,unlock_conditions_);
    serialize_list<quint8>(out,features_);
    serialize_list<quint8>(out,immutable_features_);
}

std::shared_ptr<Output> Output::Alias(const quint64& amount_m, const pvector<const Unlock_Condition> &unlock_conditions_m,
                                      const fl_array<quint16>& state_metadata_m, const quint32& foundry_counter_m,
                                      const quint32& state_index_m,
                                      const pvector<const Native_Token> &native_tokens_m,
                                      const pvector<const Feature> &immutable_features_m,
                                      const pvector<const Feature> &features_m)
{
    return std::shared_ptr<Output>(new Alias_Output(amount_m,unlock_conditions_m,state_metadata_m,foundry_counter_m,state_index_m,native_tokens_m,immutable_features_m,features_m));
}

Alias_Output::Alias_Output(const quint64& amount_m, const pvector<const Unlock_Condition> &unlock_conditions_m,
                           const fl_array<quint16>& state_metadata_m, const quint32& foundry_counter_m,
                           const quint32& state_index_m,
                           const pvector<const Native_Token> &native_tokens_m,
                           const pvector<const Feature> &immutable_features_m,
                           const pvector<const Feature> &features_m):
    Output(types::Alias_typ,amount_m,
           unlock_conditions_m,
           features_m,
           native_tokens_m,
           immutable_features_m),
    alias_id_(Alias_ID(32,0)),state_index_(state_index_m),foundry_counter_(foundry_counter_m),state_metadata_(state_metadata_m)
{};


Alias_Output::Alias_Output(const QJsonValue& val):Output(types::Alias_typ,val),
    alias_id_(Alias_ID(val.toObject()["aliasId"])),state_index_(val.toObject()["stateIndex"].toInteger()),
    foundry_counter_(val.toObject()["foundryCounter"].toInteger()),state_metadata_(fl_array<quint16>(val.toObject()["stateMetadata"]))
{}

Alias_Output::Alias_Output(QDataStream &in):Output(types::Alias_typ)
{
    in>>amount_;
    native_tokens_=deserialize_list<quint8,Native_Token>(in);
    alias_id_=Alias_ID(32,0);
    in>>alias_id_;
    in>>state_index_>>state_metadata_>>foundry_counter_;
    unlock_conditions_=deserialize_list<quint8,Unlock_Condition>(in);
    features_=deserialize_list<quint8,Feature>(in);
    immutable_features_=deserialize_list<quint8,Feature>(in);
}

QJsonObject Alias_Output::get_Json(void) const
{
    auto var=this->Output::get_Json();

    var.insert("aliasId",alias_id_.toHexString());
    var.insert("stateIndex",(int)state_index_);
    var.insert("foundryCounter",(int)foundry_counter_);
    var.insert("stateMetadata",state_metadata_.toHexString());
    return var;
}
void Alias_Output::serialize(QDataStream &out)const
{

    out<<type()<<amount_;
    serialize_list<quint8>(out,native_tokens_);
    out<<alias_id_<<state_index_<<state_metadata_<<foundry_counter_;
    serialize_list<quint8>(out,unlock_conditions_);
    serialize_list<quint8>(out,features_);
    serialize_list<quint8>(out,immutable_features_);
}


};

};
