#pragma once

#include "esterv/crypto/block/carray.hpp"

namespace esterv::crypto::block
{

class ContextInput : public C_Base<ContextInputType>
{
protected:
  ContextInput(ContextInputType typ) : C_Base{typ}
  {
  }

  public:
    template <class from_type> static std::shared_ptr<const ContextInput> from(from_type &val);

    [[nodiscard]] static std::shared_ptr<const ContextInput> Commitment(const ID &commitmentId);
    [[nodiscard]] static std::shared_ptr<const ContextInput> BlockIssuanceCredit(const ID &accountId);
    [[nodiscard]] static std::shared_ptr<const ContextInput> Reward(const quint16 &index);
};

class CommitmentContextInput : public ContextInput
{
    ID m_commitmentId;

    CommitmentContextInput(const ID &commitmentId)
        : ContextInput(ContextInputType::Commitment), m_commitmentId{commitmentId}
    {
    }
    CommitmentContextInput(const QJsonValue &val)
        : ContextInput(ContextInputType::Commitment), m_commitmentId{val.toObject()["commitmentId"]}
    {
    }
    CommitmentContextInput(QDataStream &in) : ContextInput(ContextInputType::Commitment)
    {
        m_commitmentId = ID(ByteSizes::transactionId, 0);
        in >> m_commitmentId;
    }

public:
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out << m_commitmentId;
    }
    void addJson(QJsonObject &var) const override
    {
        ContextInput::addJson(var);
        var.insert("commitmentId", m_commitmentId.toHexString());
    }
    [[nodiscard]] auto commitmentId(void) const
    {
        return m_commitmentId;
    }
    void setCommitmentId(const ID &commitmentId)
    {
        m_commitmentId = commitmentId;
    }
    friend class ContextInput;
};
class BlockIssuanceCreditContextInput : public ContextInput
{
    ID m_accountId;

    BlockIssuanceCreditContextInput(const ID &accountId)
        : ContextInput(ContextInputType::BlockIssuanceCredit), m_accountId{accountId}
    {
    }
    BlockIssuanceCreditContextInput(const QJsonValue &val)
        : ContextInput(ContextInputType::BlockIssuanceCredit), m_accountId{val.toObject()["accountId"]}
    {
    }
    BlockIssuanceCreditContextInput(QDataStream &in) : ContextInput(ContextInputType::BlockIssuanceCredit)
    {
        m_accountId = ID(ByteSizes::hash, 0);
        in >> m_accountId;
    }

  public:
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out << m_accountId;
    }
    void addJson(QJsonObject &var) const override
    {
        ContextInput::addJson(var);
        var.insert("accountId", m_accountId.toHexString());
    }
    [[nodiscard]] auto accountId(void) const
    {
        return m_accountId;
    }
    void setAccountId(const ID &accountId)
    {
        m_accountId = accountId;
    }
    friend class ContextInput;
};

class RewardContextInput : public ContextInput
{
    quint16 m_index;

    RewardContextInput(const quint16 &index) : ContextInput(ContextInputType::Reward), m_index{index}
    {
    }
    RewardContextInput(const QJsonValue &val)
        : ContextInput(ContextInputType::Reward), m_index{static_cast<quint16>(val.toObject()["index"].toInteger())}
    {
    }
    RewardContextInput(QDataStream &in) : ContextInput(ContextInputType::Reward)
    {
        in >> m_index;
    }

  public:
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out << m_index;
    }
    void addJson(QJsonObject &var) const override
    {
        ContextInput::addJson(var);
        var.insert("accountId", (int)m_index);
    }
    [[nodiscard]] auto index(void) const
    {
        return m_index;
    }
    void setIndex(const quint16 &index)
    {
        m_index = index;
    }
    friend class ContextInput;
};

}; // namespace qblocks
