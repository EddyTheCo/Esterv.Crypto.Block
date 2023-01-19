#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include<QJsonArray>
#include"block/carray.hpp"
#include"block/qunlock_conditions.hpp"
#include"block/qfeatures.hpp"
#include"block/qnative_tokens.hpp"
namespace qiota{
namespace qblocks{

class Output
{
public:
    enum types : quint8 { Basic_typ=3,NFT_typ=6 };
    Output(types typ,quint64 amount_m, const std::vector<std::shared_ptr<Unlock_Condition>> & unlock_conditions_m,
           const std::vector<std::shared_ptr<Feature>>& features_m,
           const std::vector<std::shared_ptr<Native_Token>>& native_tokens_m );

    Output(types typ,const QJsonValue& val);
    Output(types typ):type_m(typ){};

    template<class from_type> static std::shared_ptr<Output> from_(from_type& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    Output_ID id(void)const;
    quint64 min_deposit_of_output(const quint64 &wkey, const quint64 &wdata, const quint64 &v_byte_cost)const;

    void serialize_native_tokens(QDataStream &out)const
    {
        serialize_list<quint8>(out,native_tokens_);
    }
    void serialize_unlock_conditions(QDataStream &out)const
    {
        serialize_list<quint8>(out,unlock_conditions_);
    }
    void serialize_features_(QDataStream &out)const
    {
        serialize_list<quint8>(out,features_);
    }

    std::shared_ptr<Unlock_Condition> get_unlock_(const Unlock_Condition::types& typ)const
    {
        const auto found=std::find_if(unlock_conditions_.begin(),unlock_conditions_.end(),
                                      [typ](const auto& it){return (it->type_m==typ);});
        return (found==unlock_conditions_.end())?nullptr:*found;
    }

    std::shared_ptr<Feature> get_feature_(const Feature::types& typ)const
    {
        const auto found=std::find_if(features_.begin(),features_.end(),
                                      [typ](const auto& it){return (it->type_m==typ);});
        return (found==features_.end())?nullptr:*found;
    }


    const types type_m;
    quint64 amount_;
    std::vector<std::shared_ptr<Unlock_Condition>> unlock_conditions_;
    std::vector<std::shared_ptr<Feature>> features_;
    std::vector<std::shared_ptr<Native_Token>> native_tokens_;

};

class Basic_Output :public Output
{
public:
    Basic_Output(quint64 amount_m, const std::vector<std::shared_ptr<Unlock_Condition>> & unlock_conditions_m,
                 const std::vector<std::shared_ptr<Feature>>& features_m,
                 const std::vector<std::shared_ptr<Native_Token>>& native_tokens_m);
    Basic_Output(const QJsonValue& val);
    Basic_Output(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;


};
class NFT_Output :public Output
{
public:
    NFT_Output(quint64 amount_m, const std::vector<std::shared_ptr<Unlock_Condition>> & unlock_conditions_m,
               const std::vector<std::shared_ptr<Feature>>& features_m,
               const std::vector<std::shared_ptr<Native_Token>>& native_tokens_m,
               std::vector<std::shared_ptr<Feature>> immutable_features_m=std::vector<std::shared_ptr<Feature>>{});
    NFT_Output(const QJsonValue& val);
    NFT_Output(QDataStream &in);
    void serialize(QDataStream &out)const;
    void serialize_immutable_features_(QDataStream &out)const
    {
        serialize_list<quint8>(out,immutable_features_);
    }
    QJsonObject get_Json(void) const;
    std::vector<std::shared_ptr<Feature>> immutable_features_;
    auto nft_id(void)const{return nft_id_;}
private:
    NFT_ID nft_id_;


};

};

};
