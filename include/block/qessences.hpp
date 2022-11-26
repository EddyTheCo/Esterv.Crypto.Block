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

    Essence(quint8 typ );
    static Essence * from_Json(const QJsonValue& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    quint8 type_m;

};

class Transaction_Essence : public Essence
{
public:
    Transaction_Essence(quint64 network_id_m,std::vector<Input*> inputs_m,
                        c_array inputs_commitment_m,
                        std::vector<Output*> outputs_m,
                        Payload* payload_m);
    Transaction_Essence(const QJsonValue& val);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;


private:
    quint64 network_id_;                   //The unique value denoting whether the block was meant for mainnet, shimmer, testnet, or a private network.
    std::vector<Input*> inputs_;
    c_array inputs_commitment_;          //BLAKE2b-256 hash serving as a commitment to the serialized outputs referenced by Inputs.
    std::vector<Output*> outputs_;
    Payload* payload_;

};


};
};
