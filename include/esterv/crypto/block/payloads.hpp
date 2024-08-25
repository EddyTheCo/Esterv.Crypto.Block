#pragma once

#include "esterv/crypto/block/carray.hpp"
#include "esterv/crypto/block/context_inputs.hpp"
#include "esterv/crypto/block/inputs.hpp"
#include "esterv/crypto/block/outputs.hpp"
#include "esterv/crypto/block/unlocks.hpp"

namespace esterv::crypto::block
{
class Essence;
class Payload : public C_Base<PayloadType>
{
  protected:
    Payload(PayloadType typ) : C_Base{typ}
    {
    }

  public:
    template <class from_type> static std::shared_ptr<const Payload> from(from_type &val);

    [[nodiscard]] static std::shared_ptr<const Payload> TaggedData(const tagF &tag, const dataF &data);
    [[nodiscard]] static std::shared_ptr<const Payload> SignedTransaction(
        const std::shared_ptr<const Essence> &essence_m, const pvector<const Unlock> &unlocks_m);

    virtual c_array get_id(void) const
    {
        return c_array(32, 0);
    };
};

class TaggedDataPayload : public Payload
{
    tagF m_tag;
    dataF m_data;

    TaggedDataPayload(const tagF &tag, const dataF &data) : Payload{PayloadType::TaggedData}, m_tag{tag}, m_data{data}
    {
    }
    TaggedDataPayload(const QJsonValue &val)
        : Payload{PayloadType::TaggedData}, m_tag{val.toObject()["tag"]}, m_data{val.toObject()["data"]}
    {
    }
    TaggedDataPayload(QDataStream &in) : Payload{PayloadType::TaggedData}
    {
        in >> m_tag;
        in >> m_data;
    }

  public:
    void serialize(QDataStream &out) const override
    {
        Payload::serialize(out);
        out << m_tag;
        out << m_data;
    }
    void addJson(QJsonObject &var) const override
    {
        Payload::addJson(var);
        var.insert("tag", m_tag.toHexString());
        var.insert("data", m_data.toHexString());
    }
    [[nodiscard]] auto tag() const
    {
        return m_tag;
    }
    [[nodiscard]] auto data() const
    {
        return m_data;
    }
    void setTag(const tagF &tag)
    {
        m_tag = tag;
    }
    void setData(const dataF &data)
    {
        m_data = data;
    }

    friend class Payload;
};

class SignedTransactionPayload : public Payload
{
    std::map<ID, quint64> m_allotments;
    quint64 m_networkId;
    quint32 m_creationSlot;
    pvector<const Input> m_inputs;
    pvector<const Output> m_outputs;
    pvector<const ContextInput> m_conxtextInputs;
    std::shared_ptr<const Payload> m_payload;
    pvector<const Unlock> m_unlocks;
    fl_array<quint8> m_capabilities;

    SignedTransactionPayload(const quint64 &networkId, const quint32 &creationSlot, const pvector<const Input> &inputs,
                             const pvector<const Output> &outputs, const pvector<const Unlock> &unlocks,
                             const pvector<const ContextInput> &conxtextInputs = {},
                             const std::map<ID, quint64> &allotments = {},
                             const fl_array<quint8> capabilities = fl_array<quint8>(),
                             const std::shared_ptr<const Payload> &payload = nullptr)
        : Payload{PayloadType::SignedTransaction}, m_networkId{networkId}, m_creationSlot{creationSlot},
          m_inputs{inputs}, m_outputs{outputs}, m_unlocks{unlocks}, m_conxtextInputs{conxtextInputs},
          m_allotments{allotments}, m_capabilities{capabilities}, m_payload{payload}
    {
    }
    SignedTransactionPayload(const QJsonValue &val)
        : Payload{PayloadType::SignedTransaction},
          m_networkId{
              static_cast<quint64>((val.toObject()["transaction"].toObject())["networkId"].toString().toLongLong())},
          m_creationSlot{static_cast<quint32>((val.toObject()["transaction"].toObject())["creationSlot"].toInteger())},
          m_inputs{getTvec<Input>((val.toObject()["transaction"].toObject())["inputs"].toArray())},
          m_outputs{getTvec<Output>((val.toObject()["transaction"].toObject())["outputs"].toArray())},
          m_unlocks{getTvec<Unlock>(val.toObject()["unlocks"].toArray())},
          m_conxtextInputs{
              getTvec<ContextInput>((val.toObject()["transaction"].toObject())["contextInputs"].toArray())},
          m_capabilities{(val.toObject()["transaction"].toObject())["capabilities"]},
          m_payload{Payload::from<const QJsonValue>((val.toObject()["transaction"].toObject())["payload"])}
    {
        const auto allotments = (val.toObject()["transaction"].toObject())["allotments"].toArray();
        for (const auto &v : allotments)
        {
            const auto vObject = v.toObject();
            m_allotments.insert({c_array{vObject["accountId"].toString()}, vObject["mana"].toString().toULongLong()});
        }
    }
    SignedTransactionPayload(QDataStream &in) : Payload{PayloadType::SignedTransaction}
    {
        in >> m_networkId;
        in >> m_creationSlot;
        m_conxtextInputs = deserializeVector<quint16, ContextInput>(in);
        m_inputs = deserializeVector<quint16, Input>(in);

        quint16 length;
        auto id = ID(ByteSizes::hash, 0);
        quint64 mana;
        in >> length;
        for (auto i = 0; i < length; i++)
        {
            in >> id;
            in >> mana;
            m_allotments.insert({id, mana});
        }
        in >> m_capabilities;
        quint32 payloadLength;
        in >> payloadLength;

        if (payloadLength != 0u)
        {
            m_payload = Payload::from<QDataStream>(in);
        }
        m_outputs = deserializeVector<quint16, Output>(in);
        m_unlocks = deserializeVector<quint16, Unlock>(in);
    }

  public:
    void serialize(QDataStream &out) const override
    {
        Payload::serialize(out);
        out << m_networkId;
        out << m_creationSlot;
        serializeList<quint16>(out, m_conxtextInputs);
        serializeList<quint16>(out, m_inputs);
        quint16 lenght = m_allotments.size();
        out << lenght;
        for (const auto &v : m_allotments)
        {
            out << v.first << v.second;
        }
        out << m_capabilities;
        if (m_payload)
        {
            c_array serialized_payload;
            serialized_payload.fromObject(m_payload);
            out << static_cast<quint32>(serialized_payload.size());
            out << serialized_payload;
        }
        serializeList<quint16>(out, m_outputs);
        serializeList<quint16>(out, m_unlocks);
    }
    void addJson(QJsonObject &var) const override
    {
        Payload::addJson(var);

        QJsonObject transaction;
        transaction.insert("networkId", QString::number(m_networkId));
        transaction.insert("creationSlot", (int)m_creationSlot);
        transaction.insert("contextInputs", jsonList(m_conxtextInputs));
        transaction.insert("inputs", jsonList(m_inputs));
        transaction.insert("allotments", jsonList(m_allotments));
        var.insert("transaction", transaction);
        var.insert("unlocks", jsonList(m_unlocks));
    }
    friend class Payload;
};
}; // namespace esterv::crypto::block
