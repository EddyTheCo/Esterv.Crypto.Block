#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include"block/carray.hpp"
namespace qiota{
namespace qblocks{
class  Input
{
public:
    Input(quint8 typ);
    static Input * from_Json(const QJsonValue& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    quint8 type_m;

};

/*
 * @brief Describes an input which references an unspent transaction output to consume.
 *
 */
class UTXO_Input : public Input
{
public:
    UTXO_Input(transaction_id  transaction_id_m,quint16 transaction_output_index_m);
    UTXO_Input(const QJsonValue& val);
    void serialize(QDataStream &out)const;

    QJsonObject get_Json(void) const;


private:
quint8 type_m;
quint16 transaction_output_index_;
transaction_id transaction_id_;

};

};

};
