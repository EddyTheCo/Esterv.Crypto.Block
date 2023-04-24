#include<block/qessences.hpp>
#include<block/qpayloads.hpp>

namespace qiota{
namespace qblocks{
void Essence::serialize(QDataStream &out)const{};
QJsonObject Essence::get_Json(void) const{return QJsonObject();};
Essence::Essence(types typ ):type_m(typ){};
QByteArray Essence::get_hash(void)const
{
    c_array serializedEssence;
    serializedEssence.from_object<Essence>(*this);
    return QCryptographicHash::hash(serializedEssence, QCryptographicHash::Blake2b_256);
}
template<class from_type>  std::shared_ptr<const Essence> Essence::from_(from_type& val){
    const auto type_=get_type<quint8>(val);
    switch(type_) {

    case Transaction_typ:
        return std::shared_ptr<Essence>(new Transaction_Essence(val));
    default:
        return nullptr;

    }
}
template std::shared_ptr<const Essence> Essence::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<const Essence> Essence::from_<const QJsonValueRef>(const QJsonValueRef& val);
template std::shared_ptr<const Essence> Essence::from_<QDataStream >(QDataStream & val);

std::shared_ptr<const Essence> Essence::Transaction(const quint64 &network_id_m,
                                                    const pvector<const Input> &inputs_m,
                                                    const c_array &inputs_commitment_m,
                                                    const pvector<const Output> &outputs_m,
                                                    const std::shared_ptr<const Payload> &payload_m)
{
    return std::shared_ptr<Essence>(new Transaction_Essence(network_id_m,inputs_m,inputs_commitment_m,outputs_m,payload_m));
}
Transaction_Essence::Transaction_Essence(const quint64& network_id_m, const pvector<const Input> &inputs_m,
                                         const c_array& inputs_commitment_m,
                                         const pvector<const Output> &outputs_m,
                                         const std::shared_ptr<const Payload> &payload_m):
    Essence(Transaction_typ),network_id_(network_id_m),inputs_(inputs_m),inputs_commitment_(inputs_commitment_m),
    outputs_(outputs_m),payload_(payload_m)
{};


Transaction_Essence::Transaction_Essence(const QJsonValue& val):
    Transaction_Essence(val.toObject()["networkId"].toInt(),
    get_T<Input>(val.toObject()["inputs"].toArray()),
    c_array(val.toObject()["inputsCommitment"]),
    get_T<Output>(val.toObject()["outputs"].toArray()),
    Payload::from_<const QJsonValue>(val.toObject()["payload"])
  ){};
Transaction_Essence::Transaction_Essence(QDataStream &in):Essence(Transaction_typ)
{

    in>>network_id_;
    quint16 inputs_count;
    in>>inputs_count;
    for(auto i=0;i<inputs_count;i++)
    {
        inputs_.push_back(Input::from_<QDataStream>(in));
    }
    inputs_commitment_=c_array(32,0);
    in>>inputs_commitment_;

    quint16 outputs_count;
    in>>outputs_count;


    for(auto i=0;i<outputs_count;i++)
    {
        outputs_.push_back(Output::from_<QDataStream>(in));
    }

    quint32 payload_length;
    in>>payload_length;

    if(payload_length)
    {
        payload_=Payload::from_<QDataStream>(in);
    }

};
void Transaction_Essence::serialize(QDataStream &out)const
{
    out<<type();
    out<<network_id_;
    out<<static_cast<quint16>(inputs_.size());
    for(const auto & v: inputs_)v->serialize(out);
    out<<inputs_commitment_;
    out<<static_cast<quint16>(outputs_.size());
    for(const auto & v: outputs_)v->serialize(out);
    if(payload_)
    {
        c_array serialized_payload;
        auto buffer=QDataStream(&serialized_payload,QIODevice::WriteOnly | QIODevice::Append);
        buffer.setByteOrder(QDataStream::LittleEndian);
        payload_->serialize(buffer);
        out<<static_cast<quint32>(serialized_payload.size());
        out<<serialized_payload;
    }
    out<<static_cast<quint32>(0);
}
QJsonObject Transaction_Essence::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type());
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
