#pragma once

#include "block/carray.hpp"
#include "block/qfeatures.hpp"
#include "block/qnative_tokens.hpp"
#include "block/qtoken_scheme.hpp"
#include "block/qunlock_conditions.hpp"
#include <QByteArray>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

namespace qiota::qblocks
{

class Output : public C_Base<OutputType>
{
  public:
    Output(OutputType typ, const quint64 &amount, const pset<const Unlock_Condition> &unlockConditions,
           const pset<const Feature> &features = {});

    Output(OutputType typ, const QJsonValue &val);
    Output(OutputType typ, QDataStream &in);

    template <class from_type> static std::shared_ptr<Output> from(from_type &val);

    std::shared_ptr<Output> clone(void) const
    {
        QByteArray var;
        auto wBuffer = QDataStream(&var, QIODevice::WriteOnly | QIODevice::Append);
        wBuffer.setByteOrder(QDataStream::LittleEndian);
        serialize(wBuffer);
        auto rBuffer = QDataStream(&var, QIODevice::ReadOnly);
        rBuffer.setByteOrder(QDataStream::LittleEndian);
        return from<QDataStream>(rBuffer);
    }
    template <class... Args> static auto Basic(Args &&...args);
    template <class... Args> static auto Account(Args &&...args);
    template <class... Args> static auto Anchor(Args &&...args);
    template <class... Args> static auto Foundry(Args &&...args);
    template <class... Args> static auto NFT(Args &&...args);

    void serialize(QDataStream &out) const;
    QJsonObject getJson(void) const;
    virtual void setId(const c_array &id);
    virtual c_array getId(void) const;
    virtual void consume(void);
    quint64 minDepositOfOutput(const quint64 &wkey, const quint64 &wdata, const quint64 &v_byte_cost) const;

    std::shared_ptr<const Unlock_Condition> getUnlock(const Unlock_Condition::types &typ) const
    {
        const auto found = std::find_if(m_unlockConditions.begin(), m_unlockConditions.end(),
                                        [typ](const auto &it) { return (it->type() == typ); });
        return (found == m_unlockConditions.end()) ? nullptr : *found;
    }

    std::shared_ptr<const Feature> getFeature(const FeatureType &typ) const
    {
        const auto found =
            std::find_if(m_features.begin(), m_features.end(), [typ](const auto &it) { return (it->type() == typ); });
        return (found == m_features.end()) ? nullptr : *found;
    }
    std::shared_ptr<const Feature> getImmutableFeature(const FeatureType &typ) const
    {
        const auto found = std::find_if(m_immutableFeatures.begin(), m_immutableFeatures.end(),
                                        [typ](const auto &it) { return (it->type() == typ); });
        return (found == m_immutableFeatures.end()) ? nullptr : *found;
    }

    QBLOCK_EXPORT const static QHash<OutputType, QString> typesstr;

  private:
    quint64 m_amount;
    pset<const Unlock_Condition> m_unlockConditions;
    pset<const Feature> m_features;
};

// move to source files
class BasicOutput : public Output
{
  public:
    template <class... Args>
    BasicOutput(const quint64 &mana, Args &&...args)
        : m_mana{mana}, Output(OutputType::Basic, std::forward<Args>(args)...)
    {
    }

  private:
    quint64 m_mana;
};
class NFTOutput : public Output
{
  public:
    template <class... Args>
    NFTOutput(const quint64 &mana, const pset<const Feature> &immutablefeatures = {}, Args &&...args)
        : Output(OutputType::NFT, std::forward<Args>(args)...), m_nftId{NFTID(ByteSizes::hash, 0)}, m_mana{mana},
          m_immutablefeatures{immutablefeatures}
    {
    }
    NFTOutput(const QJsonValue &val);
    NFTOutput(QDataStream &in);
    void serialize(QDataStream &out) const;

    QJsonObject getJson(void) const;
    void setId(const OutputID &outputid)
    {
        if (m_nftId == OutputID(ByteSizes::hash, 0))
            m_nftId = outputid.hash<QCryptographicHash::Blake2b_256>();
    }
    c_array getId(void) const
    {
        return m_nftId;
    }

  private:
    NFTID m_nftId;
    pset<const Feature> m_immutableFeatures;
    quint64 m_mana;
};

class FoundryOutput : public Output
{
  public:
    template <class... Args>
    FoundryOutput(const std::shared_ptr<TokenScheme> &tokenScheme, const quint32 &serialNumber, Args &&...args);
    FoundryOutput(const QJsonValue &val);
    FoundryOutput(QDataStream &in);
    void serialize(QDataStream &out) const;
    c_array getId(void) const;
    void consume(void);
    QJsonObject getJson(void) const;

  private:
    pset<const Feature> m_immutableFeatures;
    std::shared_ptr<TokenScheme> m_tokenScheme;
    quint32 m_serialNumber;
};

class AnchorOutput : public Output
{
  public:
    AnchorOutput(const quint64 &amount, const quint64 &mana, const pset<const Unlock_Condition> &unlockConditions,
                 const quint32 &stateIndex = 0, const pset<const Feature> &immutableFeatures = {},
                 const pset<const Feature> &features = {});
    AnchorOutput(const QJsonValue &val);
    AnchorOutput(QDataStream &in);
    void serialize(QDataStream &out) const;

    QJsonObject getJson(void) const;
    void setId(const c_array &outputid)
    {
        if (m_anchorId == c_array(ByteSizes::hash, 0))
            m_anchorId = outputid.hash<QCryptographicHash::Blake2b_256>();
    };
    c_array getId(void) const
    {
        return m_anchorId;
    }
    quint32 m_stateindex;

  private:
    AnchorID m_anchorId;
    pset<const Feature> m_immutableFeatures;
};

class AccountOutput : public Output
{
  public:
    AccountOutput(const quint64 &amount, const quint64 &mana, const pset<const Unlock_Condition> &unlockConditions,
                  const quint32 &foundryCounter = 0, const pset<const Feature> &immutableFeatures = {},
                  const pset<const Feature> &features = {});
    AccountOutput(const QJsonValue &val);
    AccountOutput(QDataStream &in);
    void serialize(QDataStream &out) const;

    QJsonObject getJson(void) const;
    void setId(const c_array &outputid)
    {
        if (m_accountId == c_array(ByteSizes::hash, 0))
            m_accountId = outputid.hash<QCryptographicHash::Blake2b_256>();
    };
    c_array getId(void) const
    {
        return m_accountId;
    }
    quint32 m_foundryCounter;

  private:
    AccountID m_accountId;
    pset<const Feature> m_immutableFeatures;
};
}; // namespace qiota::qblocks
