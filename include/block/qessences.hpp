#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include<QJsonArray>
#include"block/carray.hpp"
#include"block/qoutputs.hpp"
#include"block/qinputs.hpp"


namespace qiota{
namespace qblocks{

class Payload;

class  Essence
{
public:
    enum types : quint8 { Transaction_typ=1};
    Essence(types typ );
    template<class from_type> static std::shared_ptr<Essence> from_(from_type& val);

    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    types type(void)const{return type_m;}
    QByteArray get_hash(void)const;
private:
    const types type_m;

};

class Transaction_Essence : public Essence
{
public:
    Transaction_Essence(quint64 network_id_m,const std::vector<std::shared_ptr<Input>>& inputs_m,
                        c_array inputs_commitment_m,
                        const std::vector<std::shared_ptr<Output>> &outputs_m,
                        const std::shared_ptr<Payload>& payload_m);
    Transaction_Essence(const QJsonValue& val);
    Transaction_Essence(QDataStream &in);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;


private:
    quint64 network_id_;                   //The unique value denoting whether the block was meant for mainnet, shimmer, testnet, or a private network.
    std::vector<std::shared_ptr<Input>> inputs_;
    c_array inputs_commitment_;          //BLAKE2b-256 hash serving as a commitment to the serialized outputs referenced by Inputs.
    std::vector<std::shared_ptr<Output>> outputs_;
    std::shared_ptr<Payload> payload_;

};


};
};
