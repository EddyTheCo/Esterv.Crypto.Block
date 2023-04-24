#include<block/qessences.hpp>
#include<block/qpayloads.hpp>
#include<QCryptographicHash>
namespace qiota{
namespace qblocks{
void Payload::serialize(QDataStream &out)const{};
QJsonObject Payload::get_Json(void) const{return QJsonObject();};
Payload::Payload(types type_):type_m(type_){};
template<class from_type> std::shared_ptr<const Payload> Payload::from_(from_type& val){

    const auto type_=get_type<quint32>(val);
    switch(type_) {
    case Transaction_typ:
        return std::shared_ptr<Payload>(new Transaction_Payload(val));
     case Tagged_Data_typ:
        return std::shared_ptr<Payload>(new Tagged_Data_Payload(val));
    default:
    return nullptr;

    }
}

template std::shared_ptr<const Payload> Payload::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<const Payload> Payload::from_<QDataStream >(QDataStream & val);
template std::shared_ptr<const Payload> Payload::from_<const QJsonValueRef>(const QJsonValueRef& val);

std::shared_ptr<const Payload> Payload::Tagged_Data(const tagF &tag_m, const dataF &data_m)
{
    return std::shared_ptr<Payload>(new Tagged_Data_Payload(tag_m,data_m));
}

Tagged_Data_Payload::Tagged_Data_Payload(const tagF &tag_m, const dataF &data_m):Payload(Tagged_Data_typ),tag_(tag_m),data_(data_m){};
Tagged_Data_Payload::Tagged_Data_Payload(const QJsonValue& val):
    Tagged_Data_Payload(tagF(val.toObject()["tag"]),dataF(val.toObject()["data"])){};
Tagged_Data_Payload::Tagged_Data_Payload(QDataStream &in):Payload(Tagged_Data_typ)
{
    in>>tag_;
    in>>data_;
}
void Tagged_Data_Payload::serialize(QDataStream &out)const
{
    out<<type();
    out<<tag_;
    out<<data_;
}
QJsonObject Tagged_Data_Payload::get_Json(void) const
{
    QJsonObject var;
       var.insert("type",(int)type());
       var.insert("tag",tag_.toHexString());
       var.insert("data",data_.toHexString());
       return var;
}


std::shared_ptr<const Payload> Payload::Transaction(const std::shared_ptr<const Essence>& essence_m,const pvector<const Unlock>& unlocks_m)
{
    return std::shared_ptr<Payload>(new Transaction_Payload(essence_m,unlocks_m));
}

Transaction_Payload::Transaction_Payload(const std::shared_ptr<const Essence> &essence_m, const pvector<const Unlock>  &unlocks_m):essence_(essence_m),
    unlocks_(unlocks_m),Payload(Transaction_typ){};

Transaction_Payload::Transaction_Payload(const QJsonValue& val):
    Transaction_Payload(Essence::from_<const QJsonValue>(val.toObject()["essence"]),
    get_T<Unlock>(val.toObject()["unlocks"].toArray())
    ){};
Transaction_Payload::Transaction_Payload(QDataStream &in):Payload(Transaction_typ),essence_(Essence::from_<QDataStream>(in))
{
    quint16  unlocks_count;
    in>>unlocks_count;
    for(auto i=0;i<unlocks_count;i++)
    {
        unlocks_.push_back(Unlock::from_<QDataStream>(in));
    }
}
void Transaction_Payload::serialize(QDataStream &out)const
{
    out<<type();
    essence_->serialize(out);
    out<<static_cast<quint16>(unlocks_.size());
    for(const auto & v: unlocks_)v->serialize(out);
}
QJsonObject Transaction_Payload::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type());
    var.insert("essence",essence_->get_Json());
    QJsonArray unloArray;
    for(const auto& v: unlocks_)unloArray.push_back(v->get_Json());
    
    var.insert("unlocks",unloArray);
   
    return var;
}
c_array Transaction_Payload::get_id(void)const
{
    c_array var;
    var.from_object(*this);
    return QCryptographicHash::hash(var, QCryptographicHash::Blake2b_256);
}
};
};
