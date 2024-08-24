#include "esterv/crypto/block/inputs.hpp"

namespace esterv::crypto::block
{

template <class from_type> std::shared_ptr<const Input> Input::from(from_type &val)
{
    const auto type = getType<InputType>(val);

    switch (type)
    {

    case InputType::UTXO:
        return std::shared_ptr<Input>(new UTXOInput(val));
    default:
        return nullptr;
    }
}
template std::shared_ptr<const Input> Input::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const Input> Input::from<QDataStream>(QDataStream &val);

std::shared_ptr<const Input> Input::UTXO(const ID &transactionId, const quint16 &transactionOutputIndex)
{
    return std::shared_ptr<const Input>(new UTXOInput(transactionId, transactionOutputIndex));
}

}; // namespace qblocks
