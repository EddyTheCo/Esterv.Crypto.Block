#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include"block/carray.hpp"
#include"block/qaddresses.hpp"
namespace qiota{
namespace qblocks{
class  Feature
{
public:

    Feature(quint8 typ );
    static Feature * from_Json(const QJsonValue& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    quint8 type_m;

};

class Sender_Feature : public Feature
{
public:
    Sender_Feature(Address* sender_m);
    Sender_Feature(const QJsonValue& val);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;

private:
Address * sender_;

};

class Metadata_Feature : public Feature
{
public:
    Metadata_Feature(fl_array<quint16> data_m);
    Metadata_Feature(const QJsonValue& val);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;


private:
fl_array<quint16> data_;

};

class Tag_Feature: public Feature
{
public:
    Tag_Feature(fl_array<quint8> tag_m);
    Tag_Feature(const QJsonValue& val);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;


private:

fl_array<quint8> tag_;

};
};
};
