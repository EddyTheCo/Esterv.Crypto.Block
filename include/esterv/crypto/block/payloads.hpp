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
    std::shared_ptr<const TaggedDataPayload> m_payload;
    pvector<const Unlock> m_unlocks;

    SignedTransactionPayload(const std::shared_ptr<const Essence> &essence_m, const pvector<const Unlock> &unlocks_m);
    SignedTransactionPayload(const QJsonValue &val);
    SignedTransactionPayload(QDataStream &in);

  public:
    void serialize(QDataStream &out) const override
    {
    }
    void addJson(QJsonObject &var) const override
    {
    }
};
}; // namespace esterv::crypto::block
