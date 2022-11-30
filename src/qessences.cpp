#include<block/qessences.hpp>
#include<block/qpayloads.hpp>

namespace qiota{
namespace qblocks{
void Essence::serialize(QDataStream &out)const{};
QJsonObject Essence::get_Json(void) const{return QJsonObject();};
Essence::Essence(quint8 typ ):type_m(typ){};
Essence * Essence::from_Json(const QJsonValue& val){
    const quint8 type_=val.toObject()["type"].toInt();

    switch(type_) {

      case 1:
        return new Transaction_Essence(val);
    default:
    return nullptr;

    }
}

template std::vector<Input *> get_T<Input>(const QJsonArray& val);
template std::vector<Output *> get_T<Output>(const QJsonArray& val);
Transaction_Essence::Transaction_Essence(quint64 network_id_m, std::vector<Input *> inputs_m,
                    c_array inputs_commitment_m,
                    std::vector<Output *> outputs_m,
                    Payload* payload_m):Essence(1),network_id_(network_id_m),inputs_(inputs_m),inputs_commitment_(inputs_commitment_m),
    outputs_(outputs_m),payload_(payload_m)
{};


Transaction_Essence::Transaction_Essence(const QJsonValue& val):
    Transaction_Essence(val.toObject()["networkId"].toInt(),
    get_T<Input>(val.toObject()["inputs"].toArray()),
    c_array(val.toObject()["inputsCommitment"]),
    get_T<Output>(val.toObject()["outputs"].toArray()),
    Payload::from_Json(val.toObject()["payload"])
  ){};

void Transaction_Essence::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<network_id_;
    out<<static_cast<quint16>(inputs_.size());
    for(const auto & v: inputs_)v->serialize(out);
    out<<inputs_commitment_;
    out<<static_cast<quint16>(outputs_.size());
    for(const auto & v: outputs_)v->serialize(out);
    if(payload_)
    {
        c_array serialized_payload;
        payload_->serialize(*serialized_payload.get_buffer());
        out<<static_cast<quint32>(serialized_payload.size());
        out<<serialized_payload;
    }
    out<<static_cast<quint32>(0);
}
QJsonObject Transaction_Essence::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("networkId",QString::number(network_id_));  //check this
    var.insert("inputsCommitment",inputs_commitment_.toHexString());

    QJsonArray inarr;
    for(const auto& v: inputs_)inarr.push_back(v->get_Json());
    var.insert("inputs",inarr);

    QJsonArray outarr;
    for(const auto& v: outputs_)outarr.push_back(v->get_Json());
    var.insert("outputs",outarr);
    if(payload_)
    {
        var.insert("payload",payload_->get_Json());
    }
    else
    {
        var.insert("payload",QJsonValue::Null);
    }

    return var;
}



};
};
