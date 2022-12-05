#include<block/qessences.hpp>
#include<block/qpayloads.hpp>
namespace qiota{
namespace qblocks{
void Payload::serialize(QDataStream &out)const{};
QJsonObject Payload::get_Json(void) const{return QJsonObject();};
Payload::Payload(quint32 type_):type_m(type_){};
template<class from_type> std::shared_ptr<Payload> Payload::from_(from_type& val){

    const auto type_=get_type<quint32>(val);
    switch(type_) {
    case 6:
        return std::shared_ptr<Payload>(new Transaction_Payload(val));
     case 5:
        return std::shared_ptr<Payload>(new Tagged_Data_Payload(val));
    default:
    return nullptr;

    }
}

template std::shared_ptr<Payload> Payload::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<Payload> Payload::from_<QDataStream >(QDataStream & val);
template std::shared_ptr<Payload> Payload::from_<const QJsonValueRef>(const QJsonValueRef& val);

Tagged_Data_Payload::Tagged_Data_Payload(const tagF tag_m, const dataF data_m):Payload(5),tag_(tag_m),data_(data_m){};
Tagged_Data_Payload::Tagged_Data_Payload(const QJsonValue& val):
    Tagged_Data_Payload(tagF(val.toObject()["tag"]),dataF(val.toObject()["data"])){};
Tagged_Data_Payload::Tagged_Data_Payload(QDataStream &in):Payload(5)
{
    qDebug()<<"inside Tagged_Data_Payload";
    in>>tag_;
    in>>data_;

}
void Tagged_Data_Payload::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<tag_;
    out<<data_;
}
QJsonObject Tagged_Data_Payload::get_Json(void) const
{
    QJsonObject var;
       var.insert("type",(int)type_m);
       var.insert("tag",tag_.toHexString());
       var.insert("data",data_.toHexString());
       return var;
}

Transaction_Payload::Transaction_Payload(const std::shared_ptr<Essence>& essence_m,const std::vector<std::shared_ptr<Unlock>>& unlocks_m):essence_(essence_m),
    unlocks_(unlocks_m),Payload(6){};

Transaction_Payload::Transaction_Payload(const QJsonValue& val):
    Transaction_Payload(Essence::from_<const QJsonValue>(val.toObject()["essence"]),
    get_T<Unlock>(val.toObject()["unlocks"].toArray())
    ){};
Transaction_Payload::Transaction_Payload(QDataStream &in):Payload(6),essence_(Essence::from_<QDataStream>(in))
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
    out<<type_m;
    essence_->serialize(out);
    out<<static_cast<quint16>(unlocks_.size());
    for(const auto & v: unlocks_)v->serialize(out);
}
QJsonObject Transaction_Payload::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("essence",essence_->get_Json());
    QJsonArray unloArray;
    for(const auto& v: unlocks_)unloArray.push_back(v->get_Json());
    
    var.insert("unlocks",unloArray);
   
    return var;
}

};
};
