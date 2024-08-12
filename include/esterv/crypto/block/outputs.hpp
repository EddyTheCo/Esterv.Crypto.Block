#pragma once

#include "esterv/crypto/block/features.hpp"
#include "esterv/crypto/block/unlock_conditions.hpp"

namespace esterv::crypto::block
{

class Output : virtual public C_Base<OutputType>
{

    quint64 m_amount;

    QBLOCK_EXPORT static const QHash<OutputType, QString> typesstr;

protected:
    pset<const UnlockCondition> m_unlockConditions;

    Output(const quint64 &amount = 0, const pset<const UnlockCondition> &unlockConditions = {})
        : C_Base<OutputType>{OutputType::Basic}, m_amount{amount}, m_unlockConditions{unlockConditions}
    {
    }
    Output(const QJsonValue &val)
        : C_Base<OutputType>{OutputType::Basic}, m_amount{val.toObject()["amount"].toString().toULongLong()},
          m_unlockConditions{getT<UnlockCondition>(val.toObject()["unlockConditions"].toArray())}
    {

    }
    Output(QDataStream &in) : C_Base<OutputType>(OutputType::Basic)
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

class BasicOutput : public Output
{
    quint64 m_mana;

  protected:
    pset<const Feature> m_features;
    template <class... Args>
    BasicOutput(const quint64 &mana = 0, const pset<const Feature> &features = {}, Args &&...args)
        : m_mana{mana}, m_features{features}, Output(std::forward<Args>(args)...), C_Base<OutputType>{OutputType::Basic}
    {
    }
    BasicOutput(const QJsonValue &val)
        : Output(val), C_Base<OutputType>{OutputType::Basic}, m_mana{val.toObject()["mana"].toString().toULongLong()},
          m_features{getT<Feature>(val.toObject()["features"].toArray())}
    {
    }
    BasicOutput(QDataStream &in) : Output(in), C_Base<OutputType>{OutputType::Basic}
    {
        in>>m_mana;
        if (type() == OutputType::Basic)
        {
            m_unlockConditions = deserializeList<quint8, UnlockCondition>(in);
            m_features = deserializeList<quint8, Feature>(in);
        }
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
        if (type() == OutputType::Basic)
        {
            serializeList<quint8>(out, m_unlockConditions);
            serializeList<quint8>(out, m_features);
        }
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

class ChainOutput : public BasicOutput
{

    ID m_id;

    QBLOCK_EXPORT static const QHash<OutputType, QString> jsonStr;

  protected:
    pset<const Feature> m_immutableFeatures;
    template <class... Args>
    ChainOutput(const ID &id = ID(ByteSizes::hash, 0), const pset<const Feature> &immutablefeatures = {},
                Args &&...args)
        : C_Base<OutputType>{OutputType::NFT}, m_id{ByteSizes::hash, 0}, m_immutableFeatures{immutablefeatures},
          BasicOutput{std::forward<Args>(args)...}
    {
    }
    ChainOutput(const QJsonValue &val)
        : C_Base<OutputType>{OutputType::NFT}, m_id{val.toObject()[jsonStr[m_type]]},
          m_immutableFeatures{getT<Feature>(val.toObject()["immutableFeatures"].toArray())}, BasicOutput(val)
    {
    }
    ChainOutput(QDataStream &in) : C_Base<OutputType>(OutputType::NFT), BasicOutput(in)
    {
        m_id = ID(ByteSizes::hash, 0);
        in >> m_id;
    }
    void serialize(QDataStream &out) const override
    {
        BasicOutput::serialize(out);
        out << m_id;
    }
    void finishSerialize(QDataStream &out) const
    {
        serializeList<quint8>(out, m_unlockConditions);
        serializeList<quint8>(out, m_features);
        serializeList<quint8>(out, m_immutableFeatures);
    }
    void finishDeserialize(QDataStream &in)
    {
        m_unlockConditions = deserializeList<quint8, UnlockCondition>(in);
        m_features = deserializeList<quint8, Feature>(in);
        m_immutableFeatures = deserializeList<quint8, Feature>(in);
    }
    void addJson(QJsonObject &var) const override
    {
        BasicOutput::addJson(var);
        var.insert(jsonStr[m_type], m_id.toHexString());
        if (m_immutableFeatures.size())
        {
            QJsonArray features;
            for (const auto &v : m_immutableFeatures)
            {
                QJsonObject feature;
                v->addJson(feature);
                features.push_back(feature);
            }
            var.insert("immutableFeatures", features);
        }
    }

  public:
    void setId(const ID &outputId)
    {
        if (m_id == ID(ByteSizes::hash, 0))
            m_id = m_id.hash<QCryptographicHash::Blake2b_256>();
    }
    [[nodiscard]] auto Id(void) const
    {
        return m_id;
    }
    [[nodiscard]] std::shared_ptr<const Feature> getImmutableFeature(const FeatureType &typ) const
    {
        const auto found = std::find_if(m_immutableFeatures.begin(), m_features.end(),
                                        [typ](const auto &it) { return (it->type() == typ); });
        return (found == m_features.end()) ? nullptr : *found;
    }
    void setImmutableFeature(const std::shared_ptr<const Feature> &feature)
    {
        m_immutableFeatures.insert(feature);
    }
};
class NFTOutput : public ChainOutput
{
    template <class... Args>
    NFTOutput(Args &&...args) : ChainOutput(std::forward<Args>(args)...), C_Base<OutputType>{OutputType::NFT}
    {
    }
    NFTOutput(const QJsonValue &val) : C_Base<OutputType>{OutputType::NFT}, ChainOutput(val)

    {
    }
    NFTOutput(QDataStream &in) : C_Base<OutputType>{OutputType::NFT}, ChainOutput(in)
    {
        ChainOutput::finishDeserialize(in);
    }

  public:
    void serialize(QDataStream &out) const override
    {
        ChainOutput::serialize(out);
        ChainOutput::finishSerialize(out);
    }
};

class AnchorOutput : public ChainOutput
{
    quint32 m_stateIndex;
    template <class... Args>
    AnchorOutput(const quint32 &stateIndex = 0, Args &&...args)
        : ChainOutput(std::forward<Args>(args)...), C_Base<OutputType>{OutputType::Anchor}, m_stateIndex{stateIndex}
    {
    }
    AnchorOutput(const QJsonValue &val)
        : C_Base<OutputType>{OutputType::Anchor}, ChainOutput(val),
          m_stateIndex(val.toObject()["stateIndex"].toInteger())
    {
    }
    AnchorOutput(QDataStream &in) : C_Base<OutputType>{OutputType::Anchor}, ChainOutput(in)
    {
        in >> m_stateIndex;
        ChainOutput::finishDeserialize(in);
    }

  public:
    void serialize(QDataStream &out) const override
    {
        ChainOutput::serialize(out);
        out << m_stateIndex;
        ChainOutput::finishSerialize(out);
    }

    void addJson(QJsonObject &var) const override
    {
        ChainOutput::addJson(var);
        var.insert("stateIndex", (int)m_stateIndex);
    }
    [[nodiscard]] auto stateIndex() const
    {
        return m_stateIndex;
    }
    void setStateIndex(const quint32 &stateIndex)
    {
        m_stateIndex = stateIndex;
    }
};

class AccountOutput : public ChainOutput
{
    quint32 m_foundryCounter;
    template <class... Args>
    AccountOutput(const quint32 &foundryCounter = 0, Args &&...args)
        : ChainOutput(std::forward<Args>(args)...), C_Base<OutputType>{OutputType::Account}
    {
    }
    AccountOutput(const QJsonValue &val)
        : C_Base<OutputType>{OutputType::Account}, ChainOutput(val),
          m_foundryCounter(val.toObject()["foundryCounter"].toInteger())
    {
    }
    AccountOutput(QDataStream &in) : C_Base<OutputType>{OutputType::Account}, ChainOutput(in)
    {
        in >> m_foundryCounter;
        ChainOutput::finishDeserialize(in);
    }

  public:
    void serialize(QDataStream &out) const override
    {
        ChainOutput::serialize(out);
        out << m_foundryCounter;
        ChainOutput::finishSerialize(out);
    }

    void addJson(QJsonObject &var) const override
    {
        ChainOutput::addJson(var);
        var.insert("foundryCounter", (int)m_foundryCounter);
    }

    [[nodiscard]] auto foundryCounter() const
    {
        return m_foundryCounter;
    }
    void setFoundryCounter(const quint32 &foundryCounter)
    {
        m_foundryCounter = foundryCounter;
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
}; // namespace qiota::qblocks
