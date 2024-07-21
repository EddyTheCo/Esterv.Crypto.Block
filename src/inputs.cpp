#include <block/qinputs.hpp>
namespace qiota
{
namespace qblocks
{
void Input::serialize(QDataStream &out) const {};
QJsonObject Input::get_Json(void) const
{
    return QJsonObject();
};
Input::Input(types typ) : type_m(typ){};

template <class from_type> std::shared_ptr<const Input> Input::from_(from_type &val)
{
    const auto type_ = get_type<quint8>(val);

    switch (type_)
    {

    case UTXO_typ:
        return std::shared_ptr<Input>(new UTXO_Input(val));
    default:
        return nullptr;
    }
}
template std::shared_ptr<const Input> Input::from_<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const Input> Input::from_<QDataStream>(QDataStream &val);
template std::shared_ptr<const Input> Input::from_<const QJsonValueRef>(const QJsonValueRef &val);
template std::shared_ptr<const Input> Input::from_<QJsonValueConstRef const>(QJsonValueConstRef const &);

std::shared_ptr<const Input> Input::UTXO(const Transaction_ID &transaction_id_m,
                                         const quint16 &transaction_output_index_m)
{
    return std::shared_ptr<Input>(new UTXO_Input(transaction_id_m, transaction_output_index_m));
}

UTXO_Input::UTXO_Input(const Transaction_ID &transaction_id_m, const quint16 &transaction_output_index_m)
    : Input(UTXO_typ), transaction_id_(transaction_id_m), transaction_output_index_(transaction_output_index_m){};
UTXO_Input::UTXO_Input(const QJsonValue &val)
    : UTXO_Input(Transaction_ID(val.toObject()["transactionId"]), val.toObject()["transactionOutputIndex"].toInt()){};
UTXO_Input::UTXO_Input(QDataStream &in) : Input(UTXO_typ)
{
    transaction_id_ = Transaction_ID(32, 0);
    in >> transaction_id_;
    in >> transaction_output_index_;
};
void UTXO_Input::serialize(QDataStream &out) const
{
    out << type();
    out << transaction_id_;
    out << transaction_output_index_;
}
QJsonObject UTXO_Input::get_Json(void) const
{
    QJsonObject var;
    var.insert("type", (int)type());
    var.insert("transactionId", transaction_id_.toHexString());
    var.insert("transactionOutputIndex", (int)transaction_output_index_);
    return var;
}
}; // namespace qblocks
}; // namespace qiota
