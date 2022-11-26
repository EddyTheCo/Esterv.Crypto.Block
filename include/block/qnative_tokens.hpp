#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include"block/carray.hpp"
namespace qiota{


namespace qblocks{

class Native_Token
{
public:
    Native_Token();
    Native_Token(const QJsonValue& val);
    static Native_Token * from_Json(const QJsonValue& val);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;

private:
    token_id token_id_;
     quint64 amount_;   //fixthis to uint256

};

};

};
