#include<block/qinputs.hpp>
namespace qiota{
namespace qblocks{
void Input::serialize(QDataStream &out)const{};
QJsonObject Input::get_Json(void) const{return QJsonObject();};
Input::Input(quint8 typ ):type_m(typ){};

Input * Input::from_Json(const QJsonValue& val){
    const quint8 type_=val.toObject()["type"].toInt();

    switch(type_) {

      case 0:
        return new UTXO_Input(val);
    default:
    return nullptr;

    }
}

UTXO_Input::UTXO_Input(transaction_id transaction_id_m,quint16 transaction_output_index_m):Input(0),transaction_id_(transaction_id_m),
transaction_output_index_(transaction_output_index_m){};
UTXO_Input::UTXO_Input(const QJsonValue& val):UTXO_Input(transaction_id(val.toObject()["transactionId"]),
    val.toObject()["transactionOutputIndex"].toInt()){};
void UTXO_Input::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<transaction_id_;
    out<<transaction_output_index_;
}
QJsonObject UTXO_Input::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("transactionId",transaction_id_.toHexString());
    var.insert("transactionOutputIndex",(int)transaction_output_index_);
    return var;
}
};
};
