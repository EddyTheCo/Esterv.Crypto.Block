#pragma once

#include "esterv/crypto/block/features.hpp"
#include "esterv/crypto/block/token_scheme.hpp"
#include "esterv/crypto/block/unlock_conditions.hpp"

namespace esterv::crypto::block
{

class Output : virtual public C_Base<OutputType>
{

    quint64 m_amount;

    QBLOCK_EXPORT static const QHash<OutputType, QString> typesStr;

  protected:
    pset<const UnlockCondition> m_unlockConditions;

    Output(const quint64 &amount = 0, const pset<const UnlockCondition> &unlockConditions = {})
        : C_Base{OutputType::Basic}, m_amount{amount}, m_unlockConditions{unlockConditions}
    {
    }
    Output(const QJsonValue &val)
        : C_Base{OutputType::Basic}, m_amount{val.toObject()["amount"].toString().toULongLong()},
          m_unlockConditions{getT<UnlockCondition>(val.toObject()["unlockConditions"].toArray())}
    {

    }
    Output(QDataStream &in) : C_Base<OutputType>(OutputType::Basic)
    {
        in>>m_amount;
    }

  public:
    template <class from_type> static std::shared_ptr<Output> from(from_type &val);

    [[nodiscard]] std::shared_ptr<Output> clone(void) const;

    [[nodiscard]] static std::shared_ptr<Output> Basic(const quint64 &amount, const quint64 &mana,
                                                       const pset<const UnlockCondition> &unlockConditions,
                                                       const pset<const Feature> &features = {});
    [[nodiscard]] static std::shared_ptr<Output> Account(const quint64 &amount, const quint64 &mana,
                                                         const quint32 &foundryCounter,
                                                         const pset<const UnlockCondition> &unlockConditions,
                                                         const pset<const Feature> &features = {},
                                                         const pset<const Feature> &immutableFeatures = {});
    [[nodiscard]] static std::shared_ptr<Output> Anchor(const quint64 &amount, const quint64 &mana,
                                                        const quint32 &stateIndex,
                                                        const pset<const UnlockCondition> &unlockConditions,
                                                        const pset<const Feature> &features = {},
                                                        const pset<const Feature> &immutableFeatures = {});
    [[nodiscard]] static std::shared_ptr<Output> Foundry(const quint64 &amount, const quint32 &serialNumber,
                                                         const std::shared_ptr<const TokenScheme> &tokenScheme,
                                                         const pset<const UnlockCondition> &unlockConditions,
                                                         const pset<const Feature> &features = {},
                                                         const pset<const Feature> &immutableFeatures = {});
    [[nodiscard]] static std::shared_ptr<Output> NFT(const quint64 &amount, const quint64 &mana,
                                                     const pset<const UnlockCondition> &unlockConditions,
                                                     const pset<const Feature> &features = {},
                                                     const pset<const Feature> &immutableFeatures = {});
    [[nodiscard]] static std::shared_ptr<Output> Delegation(const quint64 &amount, const quint64 &delegatedAmount,
                                                            const std::shared_ptr<const Address> &validatorAddress,
                                                            const quint64 &startEpoch, const quint64 &endEpoch,
                                                            const pset<const UnlockCondition> &unlockConditions);

    [[nodiscard]] quint64 minDepositOfOutput(const quint64 &wkey, const quint64 &wdata, const quint64 &v_byte_cost) const;

    void addJson(QJsonObject &var) const override
    {
        C_Base::addJson(var);
        var.insert("amount", QString::number(m_amount));
        QJsonArray unlockConditions;
        for (const auto &v : m_unlockConditions)
        {
            if (v)
            {
                QJsonObject unlockCondition;
                v->addJson(unlockCondition);
                unlockConditions.push_back(unlockCondition);
            }
        }
        var.insert("unlockConditions", unlockConditions);
    }
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out << m_amount;
    }

    [[nodiscard]] std::shared_ptr<const UnlockCondition> getUnlock(const UnlockConditionType &typ) const
    {
        const auto found = std::find_if(m_unlockConditions.begin(), m_unlockConditions.end(),
                                        [typ](const auto &it) { return (it->type() == typ); });
        return (found == m_unlockConditions.end()) ? nullptr : *found;
    }
    void setUnlock(std::shared_ptr<const UnlockCondition>& unlockCondition)
    {
        if (unlockCondition)
        {
            m_unlockConditions.insert(unlockCondition);
        }
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
    BasicOutput(const quint64 &mana, const pset<const Feature> &features, Args &&...args)
        : m_mana{mana}, m_features{features}, Output(std::forward<Args>(args)...), C_Base{OutputType::Basic}
    {
    }
    BasicOutput(const QJsonValue &val)
        : Output(val), C_Base{OutputType::Basic},
          m_mana{(m_type != OutputType::Foundry)
                     ? val.toObject()[(m_type != OutputType::Delegation) ? "mana" : "delegatedAmount"]
                           .toString()
                           .toULongLong()
                     : 0},
          m_features{getT<Feature>(val.toObject()["features"].toArray())}
    {
    }
    BasicOutput(QDataStream &in) : Output(in), C_Base{OutputType::Basic}
    {
        if (m_type != OutputType::Foundry)
        {
            in >> m_mana;
        }
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
        if (m_type != OutputType::Foundry && m_type != OutputType::Delegation)
        {
            var.insert((m_type != OutputType::Delegation) ? "mana" : "delegatedAmount", QString::number(m_mana));
        }
        if(m_features.size())
        {
            QJsonArray features;
            for (const auto &v : m_features)
            {
                if (v)
                {
                    QJsonObject feature;
                    v->addJson(feature);
                    features.push_back(feature);
                }
            }
            var.insert("features", features);
        }
    }
    void serialize(QDataStream &out) const override
    {
        Output::serialize(out);
        if (m_type != OutputType::Foundry)
        {
            out << m_mana;
        }
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
        if (feature)
        {
            m_features.insert(feature);
        }
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
    ChainOutput(const pset<const Feature> &immutablefeatures, Args &&...args)
        : BasicOutput{std::forward<Args>(args)...}, C_Base{OutputType::NFT}, m_id{ByteSizes::hash, 0},
          m_immutableFeatures{immutablefeatures}

    {
    }
    ChainOutput(const QJsonValue &val)
        : BasicOutput(val), C_Base{OutputType::NFT}, m_id{val.toObject()[jsonStr[m_type]]},
          m_immutableFeatures{getT<Feature>(val.toObject()["immutableFeatures"].toArray())}
    {
    }
    ChainOutput(QDataStream &in) : BasicOutput(in), C_Base{OutputType::NFT}
    {
        if (m_type != OutputType::Foundry)
        {
            m_id = ID(ByteSizes::hash, 0);
            in >> m_id;
        }
    }
    void serialize(QDataStream &out) const override
    {
        BasicOutput::serialize(out);
        if (m_type != OutputType::Foundry)
        {
            out << m_id;
        }
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
        if (m_type != OutputType::Foundry)
        {
            var.insert(jsonStr[m_type], m_id.toHexString());
        }
        if (m_immutableFeatures.size())
        {
            QJsonArray features;
            for (const auto &v : m_immutableFeatures)
            {
                if (v)
                {
                    QJsonObject feature;
                    v->addJson(feature);
                    features.push_back(feature);
                }
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
    NFTOutput(Args &&...args) : ChainOutput(std::forward<Args>(args)...), C_Base{OutputType::NFT}
    {
    }
    NFTOutput(const QJsonValue &val) : C_Base{OutputType::NFT}, ChainOutput(val)

    {
    }
    NFTOutput(QDataStream &in) : C_Base{OutputType::NFT}, ChainOutput(in)
    {
        ChainOutput::finishDeserialize(in);
    }

  public:
    void serialize(QDataStream &out) const override
    {
        ChainOutput::serialize(out);
        ChainOutput::finishSerialize(out);
    }
    friend class Output;
};

class AnchorOutput : public ChainOutput
{
    quint32 m_stateIndex;
    template <class... Args>
    AnchorOutput(const quint32 &stateIndex = 0, Args &&...args)
        : ChainOutput(std::forward<Args>(args)...), C_Base{OutputType::Anchor}, m_stateIndex{stateIndex}
    {
    }
    AnchorOutput(const QJsonValue &val)
        : C_Base{OutputType::Anchor}, ChainOutput(val), m_stateIndex(val.toObject()["stateIndex"].toInteger())
    {
    }
    AnchorOutput(QDataStream &in) : C_Base{OutputType::Anchor}, ChainOutput(in)
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
    friend class Output;
};

class AccountOutput : public ChainOutput
{
    quint32 m_foundryCounter;
    template <class... Args>
    AccountOutput(const quint32 &foundryCounter = 0, Args &&...args)
        : ChainOutput(std::forward<Args>(args)...), C_Base{OutputType::Account}
    {
    }
    AccountOutput(const QJsonValue &val)
        : C_Base{OutputType::Account}, ChainOutput(val), m_foundryCounter(val.toObject()["foundryCounter"].toInteger())
    {
    }
    AccountOutput(QDataStream &in) : C_Base{OutputType::Account}, ChainOutput(in)
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
    friend class Output;
};
class FoundryOutput : public ChainOutput
{
    std::shared_ptr<const TokenScheme> m_tokenScheme;
    quint32 m_serialNumber;

    template <class... Args>
    FoundryOutput(const std::shared_ptr<const TokenScheme> &tokenScheme, const quint32 &serialNumber, Args &&...args)
        : ChainOutput{std::forward<Args>(args)...}, C_Base{OutputType::Foundry}, m_tokenScheme{tokenScheme},
          m_serialNumber{serialNumber}
    {
    }
    FoundryOutput(const QJsonValue &val)
        : ChainOutput{val}, C_Base{OutputType::Foundry},
          m_tokenScheme(TokenScheme::from<const QJsonValue>(val.toObject()["tokenScheme"])),
          m_serialNumber(val.toObject()["serialNumber"].toInteger())
    {
    }
    FoundryOutput(QDataStream &in) : ChainOutput{in}, C_Base{OutputType::Foundry}
    {
        in >> m_serialNumber;
        m_tokenScheme = TokenScheme::from<QDataStream>(in);
        ChainOutput::finishDeserialize(in);
    }
    using BasicOutput::mana;
    using BasicOutput::setMana;
    using ChainOutput::Id;
    using ChainOutput::setId;

  public:
    void serialize(QDataStream &out) const override
    {
        ChainOutput::serialize(out);
        out << m_serialNumber;
        if (m_tokenScheme)
        {
            m_tokenScheme->serialize(out);
        }
        ChainOutput::finishSerialize(out);
    }

    void addJson(QJsonObject &var) const override
    {
        ChainOutput::addJson(var);
        var.insert("serialNumber", (int)m_serialNumber);
        if (m_tokenScheme)
        {
            QJsonObject tokenScheme;
            m_tokenScheme->addJson(tokenScheme);
            var.insert("tokenScheme", tokenScheme);
        }
    }
    [[nodiscard]] auto tokenScheme(void) const
    {
        return m_tokenScheme;
    }
    [[nodiscard]] auto serialNumber(void) const
    {
        return m_serialNumber;
    }
    void setSerialNumber(const quint32 &serialNumber)
    {
        m_serialNumber = serialNumber;
    }
    void setTokenScheme(const std::shared_ptr<const TokenScheme> &tokenScheme)
    {
        if (tokenScheme)
        {
            m_tokenScheme = tokenScheme;
        }
    }
    friend class Output;
};
class DelegationOutput : public ChainOutput
{
    quint64 m_startEpoch;
    quint64 m_endEpoch;
    std::shared_ptr<const Address> m_validatorAddress;

    using BasicOutput::getFeature;
    using BasicOutput::mana;
    using BasicOutput::setFeature;
    using BasicOutput::setMana;
    using ChainOutput::getImmutableFeature;
    using ChainOutput::setImmutableFeature;

    template <class... Args>
    DelegationOutput(const std::shared_ptr<const Address> &validatorAddress, const quint64 &startEpoch,
                     const quint64 &endEpoch, Args &&...args)
        : ChainOutput{std::forward<Args>(args)...}, C_Base{OutputType::Delegation}, m_startEpoch{startEpoch},
          m_endEpoch{endEpoch}, m_validatorAddress{validatorAddress}
    {
    }
    DelegationOutput(const QJsonValue &val)
        : ChainOutput{val}, C_Base{OutputType::Delegation},
          m_startEpoch{val.toObject()["startEpoch"].toString().toULongLong()},
          m_endEpoch{val.toObject()["endEpoch"].toString().toULongLong()},
          m_validatorAddress{Address::from<const QJsonValue>(val.toObject()["validatorAddress"])}
    {
    }
    DelegationOutput(QDataStream &in)
        : ChainOutput{in}, C_Base{OutputType::Delegation}, m_validatorAddress{Address::from<QDataStream>(in)}
    {
        in >> m_startEpoch;
        in >> m_endEpoch;
        m_unlockConditions = deserializeList<quint8, UnlockCondition>(in);
    }

  public:
    void serialize(QDataStream &out) const override
    {
        ChainOutput::serialize(out);
        if (m_validatorAddress)
        {
            m_validatorAddress->serialize(out);
        }
        out << m_startEpoch;
        out << m_endEpoch;
        serializeList<quint8>(out, m_unlockConditions);
    }

    void addJson(QJsonObject &var) const override
    {
        ChainOutput::addJson(var);
        if (m_validatorAddress)
        {
            QJsonObject validatorAddress;
            m_validatorAddress->addJson(validatorAddress);
            var.insert("validatorAddress", validatorAddress);
        }
        var.insert("startEpoch", QString::number(m_startEpoch));
        var.insert("endEpoch", QString::number(m_endEpoch));
    }
    [[nodiscard]] auto delegatedAmount(void) const
    {
        return mana();
    }
    [[nodiscard]] auto startEpoch(void) const
    {
        return m_startEpoch;
    }
    [[nodiscard]] auto endEpoch(void) const
    {
        return m_endEpoch;
    }
    [[nodiscard]] auto validatorAddress(void) const
    {
        return m_validatorAddress;
    }
    void setDelegatedAmount(const quint64 &delegatedAmount)
    {
        setMana(delegatedAmount);
    }
    void setStartEpoch(const quint64 &startEpoch)
    {
        m_startEpoch = startEpoch;
    }
    void setEndEpoch(const quint64 &endEpoch)
    {
        m_endEpoch = endEpoch;
    }
    void setValidatorAddress(const std::shared_ptr<const Address> &validatorAddress)
    {
        if (validatorAddress)
        {
            m_validatorAddress = validatorAddress;
        }
    }
    friend class Output;
};
}; // namespace qiota::qblocks
