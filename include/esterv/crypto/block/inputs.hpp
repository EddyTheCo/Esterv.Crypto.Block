#pragma once

#include "esterv/crypto/block/carray.hpp"

namespace esterv::crypto::block
{

class Input: public C_Base<InputType>
{
protected:
  Input(InputType typ) : C_Base{typ}
  {
  }

  public:
    template <class from_type> static std::shared_ptr<const Input> from(from_type &val);

    [[nodiscard]] static std::shared_ptr<const Input> UTXO(const TransactionID &transactionId,
                                             const quint16 &transactionOutputIndex);

};

/*
 * @brief Describes an input which references an unspent transaction output to consume.
 *
 */
class UTXOInput : public Input
{
    TransactionID m_transactionId;
    quint16 m_transactionOutputIndex;

    UTXOInput(const TransactionID &transactionId,
               const quint16 &transactionOutputIndex) : Input(InputType::UTXO),
        m_transactionId{transactionId},
        m_transactionOutputIndex{transactionOutputIndex}
    {
    }
    UTXOInput(const QJsonValue &val)
        : Input{InputType::UTXO}, m_transactionId{TransactionID(val.toObject()["transactionId"])},
        m_transactionOutputIndex(val.toObject()["transactionOutputIndex"].toInt())
    {
    }
    UTXOInput(QDataStream &in) : Input{InputType::UTXO}
    {
        m_transactionId = TransactionID(ByteSizes::transactionId, 0);
        in >> m_transactionId;
        in >> m_transactionOutputIndex;
    }

public:
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out<< m_transactionId;
        out << m_transactionOutputIndex;
    }
    void addJson(QJsonObject &var) const override
    {
        Input::addJson(var);
        var.insert("transactionId", m_transactionId.toHexString());
        var.insert("transactionOutputIndex", (int)m_transactionOutputIndex);
    }
    [[nodiscard]] auto transactionOutputIndex(void) const
    {
        return m_transactionOutputIndex;
    }
    [[nodiscard]] auto transactionId(void) const
    {
        return m_transactionId;
    }
    void setTransactionId(const TransactionID &transactionId)
    {
        m_transactionId = transactionId;
    }
    void setTransactionOutputIndex(const quint16 &transactionOutputIndex)
    {
        m_transactionOutputIndex = transactionOutputIndex;
    }
    friend class Input;
};

}; // namespace qblocks
