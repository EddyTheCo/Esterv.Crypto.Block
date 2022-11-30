#include<block/qessences.hpp>
#include<block/qpayloads.hpp>
namespace qiota{
namespace qblocks{
void Payload::serialize(QDataStream &out)const{};
QJsonObject Payload::get_Json(void) const{return QJsonObject();};
Payload::Payload(quint32 type_):type_m(type_){};
Payload * Payload::from_Json(const QJsonValue& val){
    const quint8 type_=val.toObject()["type"].toInt();
    switch(type_) {
    case 6:
        return new Transaction_Payload(val);
     case 5:
        return new Tagged_Data_Payload(val);
    default:
    return nullptr;

    }
}

Tagged_Data_Payload::Tagged_Data_Payload(const tagF tag_m, const dataF data_m):Payload(5),tag_(tag_m),data_(data_m){};
Tagged_Data_Payload::Tagged_Data_Payload(const QJsonValue& val):
    Tagged_Data_Payload(tagF(val.toObject()["tag"]),dataF(val.toObject()["data"])){};

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

Transaction_Payload::Transaction_Payload(Essence* essence_m,std::vector<Unlock*> unlocks_m):essence_(essence_m),
    unlocks_(unlocks_m),Payload(6){};

Transaction_Payload::Transaction_Payload(const QJsonValue& val):
    Transaction_Payload(Essence::from_Json(val.toObject()["essence"]),
    get_T<Unlock>(val.toObject()["unlocks"].toArray())
    ){};

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
