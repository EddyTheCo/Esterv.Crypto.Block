#include"block/qblock.hpp"
#include<QCryptographicHash>

namespace qiota{
namespace qblocks{

QByteArray Block::get_inputs_Commitment(QByteArray Inputs_hash)
{
    return QCryptographicHash::hash(Inputs_hash, QCryptographicHash::Blake2b_256);
}
Block::Block(const std::shared_ptr<Payload> &pyl):
    payload_(pyl){};

Block::Block(QDataStream &in)
{

    in>>protocol_version;
    quint8  parents_count;
    in>>parents_count;
    for(auto i=0;i<parents_count;i++)
    {
        auto block_id_=Block_ID(32,0);
        in>>block_id_;
        parents_.push_back(block_id_);
    }
    quint32  payload_length;
    in>>payload_length;
    if(payload_length)
    {
        payload_=Payload::from_<QDataStream>(in);
    }
    in>>nonce_;
}
Block::Block(const QJsonValue& val):protocol_version(val.toObject()["protocolVersion"].toInt()),
    nonce_(val.toObject()["nonce"].toInteger()), payload_(Payload::from_<const QJsonValue>(val))
{
    const auto arr=val.toObject()["outputs"].toArray();
    for(const auto& v:arr)parents_.push_back(Block_ID(v));
}
void Block::set_pv(const quint8& pv)
{
    protocol_version=pv;
}
void Block::set_parents(const std::vector<Block_ID> &parents_m){
    parents_ = std::move(parents_m);
}
void Block::set_nonce(const quint64& nonce_m){
    nonce_=nonce_m;
}
QJsonObject Block::get_Json(void)const
{
    QJsonObject var;
    var.insert("protocolVersion",(int)protocol_version);
    QJsonArray parentarr;
    for(const auto& v: parents_)parentarr.push_back(v.toHexString());
    var.insert("parents",parentarr);
    if(payload_)
    {
        var.insert("payload",payload_->get_Json());

    }
    else
    {
        var.insert("payload",QJsonValue::Null);
    }
    var.insert("nonce",QString::number(nonce_));
    return var;
}

QDataStream & operator < (QDataStream &out, const Block & obj)
{
    out<<obj.protocol_version<<static_cast<quint8>(obj.parents_.size());

    for(const auto& v:obj.parents_)out<<v;

    if(obj.payload_)
    {
        c_array serialized_payload;
        auto buffer=QDataStream(&serialized_payload,QIODevice::WriteOnly | QIODevice::Append);
        buffer.setByteOrder(QDataStream::LittleEndian);
        obj.payload_->serialize(buffer);
        out<<static_cast<quint32>(serialized_payload.size());
        out<<serialized_payload;
    }
    else
    {
        out<<static_cast<quint32>(0);
    }
    return out;
}

void Block::serialize(QDataStream &out)const
{
    out<(*this);
    out<<nonce_;
}

}


}
