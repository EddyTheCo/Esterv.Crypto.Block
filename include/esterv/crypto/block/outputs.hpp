#pragma once

#include "esterv/crypto/block/features.hpp"
#include "esterv/crypto/block/unlock_conditions.hpp"

namespace esterv::crypto::block
{


class Output : public C_Base<OutputType>
{

    quint64 m_amount;

    QBLOCK_EXPORT static const QHash<OutputType, QString> typesstr;

protected:
    pset<const UnlockCondition> m_unlockConditions;

    Output(OutputType typ, const quint64 &amount, const pset<const UnlockCondition> &unlockConditions):
      C_Base<OutputType>{typ}, m_amount{amount}, m_unlockConditions{unlockConditions} {
    }
    Output(OutputType typ,const QJsonValue &val):C_Base<OutputType>(typ),
        m_amount{val.toObject()["amount"].toString().toULongLong()},
        m_unlockConditions{getT<UnlockCondition>(val.toObject()["unlockConditions"].toArray())}
    {

    }
    Output(OutputType typ,QDataStream &in):C_Base<OutputType>(typ)
    {
        in>>m_amount;
    }

    void addJson(QJsonObject &var) const override
    {
        C_Base::addJson(var);
        var.insert("amount", QString::number(m_amount));
        QJsonArray unlockConditions;
        for (const auto &v : m_unlockConditions)
        {
            QJsonObject unlockCondition;
            v->addJson(unlockCondition);
            unlockConditions.push_back(unlockCondition);
        }
        var.insert("unlockConditions", unlockConditions);

    }
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out<<m_amount;
    }

  public:
    template <class from_type> static std::shared_ptr<Output> from(from_type &val);

    [[nodiscard]] std::shared_ptr<Output> clone(void) const;

    template <class... Args> [[nodiscard]] static auto Basic(Args &&...args);
    template <class... Args> [[nodiscard]] static auto Account(Args &&...args);
    template <class... Args> [[nodiscard]] static auto Anchor(Args &&...args);
    template <class... Args> [[nodiscard]] static auto Foundry(Args &&...args);
    template <class... Args> [[nodiscard]] static auto NFT(Args &&...args);
    template <class... Args> [[nodiscard]] static auto Delegation(Args &&...args);

    virtual void setId(const OutputID &id);
    [[nodiscard]] virtual OutputID getId(void) const;
    virtual void consume(void);
    [[nodiscard]] quint64 minDepositOfOutput(const quint64 &wkey, const quint64 &wdata, const quint64 &v_byte_cost) const;

    [[nodiscard]] std::shared_ptr<const UnlockCondition> getUnlock(const UnlockConditionType &typ) const
    {
        const auto found = std::find_if(m_unlockConditions.begin(), m_unlockConditions.end(),
                                        [typ](const auto &it) { return (it->type() == typ); });
        return (found == m_unlockConditions.end()) ? nullptr : *found;
    }
    void setUnlock(std::shared_ptr<const UnlockCondition>& unlockCondition)
    {
        m_unlockConditions.insert(unlockCondition);
    }
    [[nodiscard]] auto amount()const{return m_amount;}
    void setAmount(const quint64 amount){m_amount=amount;}

};

class BasicOutput : virtual public Output
{
    quint64 m_mana;
    pset<const Feature> m_features;
    template <class... Args>
    BasicOutput(const quint64 &mana, const pset<const Feature> &features = {}, Args &&...args)
        : m_mana{mana},m_features{features}, Output(OutputType::Basic, std::forward<Args>(args)...) //Check This
    {
    }
    BasicOutput(const QJsonValue &val)
        : Output(OutputType::Basic,val),m_mana{val.toObject()["mana"].toString().toULongLong()},
        m_features{getT<Feature>(val.toObject()["features"].toArray())}
    {
    }
    BasicOutput(QDataStream &in) : Output(OutputType::Basic,in)
    {
        in>>m_mana;
        m_unlockConditions = deserializeList<quint8, UnlockCondition>(in);
        m_features = deserializeList<quint8, Feature>(in);
    }
public:
    void addJson(QJsonObject &var) const override
    {
        Output::addJson(var);
        var.insert("mana", QString::number(m_mana));
        if(m_features.size())
        {
            QJsonArray features;
            for (const auto &v : m_features)
            {
                QJsonObject feature;
                v->addJson(feature);
                features.push_back(feature);
            }
            var.insert("features", features);
        }
    }
    void serialize(QDataStream &out) const override
    {
        Output::serialize(out);
        out<<m_mana;
        serializeList<quint8>(out, m_unlockConditions);
        serializeList<quint8>(out, m_features);
    }

    [[nodiscard]] auto mana()const{return m_mana;}
    void setMana(const quint64 mana){m_mana=mana;}

    [[nodiscard]] std::shared_ptr<const Feature> getFeature(const FeatureType &typ) const
    {
        const auto found = std::find_if(m_features.begin(), m_features.end(),
                                        [typ](const auto &it) { return (it->type() == typ); });
        return (found == m_features.end()) ? nullptr : *found;
    }
    void setFeature(std::shared_ptr<const Feature>& feature)
    {
        m_features.insert(feature);
    }


    friend class Output;
};
class NFTOutput : public BasicOutput
{
    NFTID m_nftId;
    pset<const Feature> m_immutableFeatures;

  public:
    template <class... Args>
    NFTOutput(const pset<const Feature> &immutablefeatures = {}, Args &&...args)
        : BasicOutput(std::forward<Args>(args)...),m_nftId{NFTID(ByteSizes::hash, 0)},
          m_immutableFeatures{immutablefeatures},Output(OutputType::NFT, std::forward<Args>(args)...)
    {
    }
    NFTOutput(const QJsonValue &val);
    NFTOutput(QDataStream &in);
    void serialize(QDataStream &out) const;

    QJsonObject getJson(void) const;
    void setId(const OutputID &outputid) override
    {
        if (m_nftId == OutputID(ByteSizes::hash, 0))
            m_nftId = outputid.hash<QCryptographicHash::Blake2b_256>();
    }
    c_array getId(void) const override
    {
        return m_nftId;
    }

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
