#include"block/qfeatures.hpp"
namespace qiota{
namespace qblocks{
void Feature::serialize(QDataStream &out)const{};
QJsonObject Feature::get_Json(void) const{return QJsonObject();};
Feature::Feature(quint8 typ ):type_m(typ){};
Feature * Feature::from_Json(const QJsonValue& val){
    const quint8 type_=val.toObject()["type"].toInt();

    switch(type_) {
    case 2:
        return new Metadata_Feature(val);
    case 3:
        return new Tag_Feature(val);
    case 0:
        return new Sender_Feature(val);
    default:
    return nullptr;

    }
}

Sender_Feature::Sender_Feature(Address* sender_m):Feature(0),sender_(sender_m){};
Sender_Feature::Sender_Feature(const QJsonValue& val):Sender_Feature(Address::from_Json(val.toObject()["sender"])){};
void Sender_Feature::serialize(QDataStream &out)const
{
    out<<type_m;
    sender_->serialize(out);
}
QJsonObject Sender_Feature::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",QString::number(type_m));
    var.insert("sender",sender_->get_Json());
    return var;
}

Metadata_Feature::Metadata_Feature(dataF data_m):Feature(2),data_(data_m){};
Metadata_Feature::Metadata_Feature(const QJsonValue& val):Metadata_Feature(dataF(val.toObject()["data"])){};
void Metadata_Feature::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<data_;
}
QJsonObject Metadata_Feature::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",QString::number(type_m));
    var.insert("data",data_.toHexString());
    return var;
}
Tag_Feature::Tag_Feature(tagF tag_m):Feature(3),tag_(tag_m){};
Tag_Feature::Tag_Feature(const QJsonValue& val):Tag_Feature(tagF(val.toObject()["tag"])){};
void Tag_Feature::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<tag_;
}
QJsonObject Tag_Feature::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",QString::number(type_m));
    var.insert("tag",tag_.toHexString());
    return var;
}
};
};
