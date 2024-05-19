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

enum ByteSizes {hash=32};
class Output
{
  public:
    enum types : quint8
    {
        Basic_typ = 0,
        Account_typ = 1,
        Anchor_typ = 2,
        Foundry_typ = 3,
        NFT_typ = 4,
        All_type=100

    };
    Output(types typ, const quint64 &amount, const pset<const Unlock_Condition> &unlock_conditions,
           const pset<const Feature> &features = {}, const pset<const Native_Token> &native_tokens = {},
           const pset<const Feature> &immutable_features = {});

    Output(types typ, const QJsonValue &val);
    Output(types typ) : type_m(typ){};

    template <class from_type> static std::shared_ptr<Output> from_(from_type &val);

    std::shared_ptr<Output> clone(void) const
    {
        QByteArray var;
        auto wBuffer = QDataStream(&var, QIODevice::WriteOnly | QIODevice::Append);
        wBuffer.setByteOrder(QDataStream::LittleEndian);
        serialize(wBuffer);
        auto rBuffer = QDataStream(&var, QIODevice::ReadOnly);
        rBuffer.setByteOrder(QDataStream::LittleEndian);
        return from_<QDataStream>(rBuffer);
    }
    static std::shared_ptr<Output> Basic(const quint64 &amount,const quint64 &mana,
                                         const pset<const Unlock_Condition> &unlockConditions,
                                         const pset<const Feature> &features = {});
    static std::shared_ptr<Output> NFT(const quint64 &amount, const quint64 &mana,
                                       const pset<const Unlock_Condition> &unlockConditions,
                                       const pset<const Feature> &immutableFeatures = {},
                                       const pset<const Feature> &features = {});
    static std::shared_ptr<Output> Foundry(const quint64 &amount,
                                           const pset<const Unlock_Condition> &unlockConditions,
                                           const std::shared_ptr<Token_Scheme> &tokenScheme,
                                           const quint32 &serialNumber,
                                           const pset<const Feature> &immutableFeatures= {},
                                           const pset<const Feature> &features = {});
    static std::shared_ptr<Output> Anchor(const quint64 &amount,const quint64 &mana,
                                         const pset<const Unlock_Condition> &unlockConditions,
                                         const quint32 &stateIndex = 0,
                                         const pset<const Feature> &immutableFeatures = {},
                                         const pset<const Feature> &features = {});
    static std::shared_ptr<Output> Account(const quint64 &amount, const quint64 &mana,
                                         const pset<const Unlock_Condition> &unlockConditions,
                                         const quint32 &foundryCounter = 0,
                                         const pset<const Feature> &immutableFeatures = {},
                                         const pset<const Feature> &features = {});


    virtual void serialize(QDataStream &out) const;
    virtual QJsonObject getJson(void) const;
    virtual void setId(const c_array &id);
    virtual c_array getId(void) const
    {
        return c_array(ByteSizes::hash, 0);
    };
    virtual void consume(void);
    quint64 minDepositOfOutput(const quint64 &wkey, const quint64 &wdata, const quint64 &v_byte_cost) const;

    std::shared_ptr<const Unlock_Condition> get_unlock_(const Unlock_Condition::types &typ) const
    {
        const auto found = std::find_if(m_unlockConditions.begin(), m_unlockConditions.end(),
                                        [typ](const auto &it) { return (it->type() == typ); });
        return (found == m_unlockConditions.end()) ? nullptr : *found;
    }

    std::shared_ptr<const Feature> get_feature_(const Feature::types &typ) const
    {
        const auto found =
            std::find_if(features_.begin(), features_.end(), [typ](const auto &it) { return (it->type() == typ); });
        return (found == features_.end()) ? nullptr : *found;
    }
    std::shared_ptr<const Feature> get_immutable_feature_(const Feature::types &typ) const
    {
        const auto found = std::find_if(immutable_features_.begin(), immutable_features_.end(),
                                        [typ](const auto &it) { return (it->type() == typ); });
        return (found == immutable_features_.end()) ? nullptr : *found;
    }

    quint64 amount_;
    pset<const Unlock_Condition> m_unlockConditions;
    pset<const Feature> m_features;
    pset<const Feature> m_immutable_features;
    types type(void) const
    {
        return m_type;
    }
    QBLOCK_EXPORT const static QHash<types, QString> typesstr;

  private:
    const types m_type;
};

class Basic_Output : public Output
{
  public:
    Basic_Output(const quint64 &amount_m, const pset<const Unlock_Condition> &unlock_conditions_m,
                 const pset<const Native_Token> &native_tokens_m = {}, const pset<const Feature> &features_m = {});
    Basic_Output(const QJsonValue &val);
    Basic_Output(QDataStream &in);
    void serialize(QDataStream &out) const;
    QJsonObject get_Json(void) const;
};
class NFT_Output : public Output
{
  public:
    NFT_Output(const quint64 &amount_m, const pset<const Unlock_Condition> &unlock_conditions_m,
               const pset<const Native_Token> &native_tokens_m = {},
               const pset<const Feature> &immutable_features_m = {}, const pset<const Feature> &features_m = {});
    NFT_Output(const QJsonValue &val);
    NFT_Output(QDataStream &in);
    void serialize(QDataStream &out) const;

    QJsonObject get_Json(void) const;
    void set_id(const c_array &outputid)
    {
        if (nft_id_ == c_array(32, 0))
            nft_id_ = outputid.hash<QCryptographicHash::Blake2b_256>();
    };
    c_array get_id(void) const
    {
        return nft_id_;
    }

  private:
    NFT_ID nft_id_;
};

class Foundry_Output : public Output
{
  public:
    Foundry_Output(const quint64 &amount_m, const pset<const Unlock_Condition> &unlock_conditions_m,
                   const std::shared_ptr<Token_Scheme> &token_scheme_m, const quint32 &serial_number_m,
                   const pset<const Native_Token> &native_tokens_m = {},
                   const pset<const Feature> &immutable_features_m = {}, const pset<const Feature> &features_m = {});
    Foundry_Output(const QJsonValue &val);
    Foundry_Output(QDataStream &in);
    void serialize(QDataStream &out) const;
    c_array get_id(void) const
    {
        c_array var;
        var = unlock_conditions_.begin()->get()->address()->addr();
        var.append(serial_number_);
        var.append(token_scheme_->type());
        return var;
    }
    void consume(void);
    QJsonObject get_Json(void) const;
    std::shared_ptr<Token_Scheme> token_scheme_;
    quint32 serial_number_;

  private:
};

class Alias_Output : public Output
{
  public:
    Alias_Output(const quint64 &amount_m, const pset<const Unlock_Condition> &unlock_conditions_m,
                 const fl_array<quint16> &state_metadata_m = "", const quint32 &foundry_counter_m = 0,
                 const quint32 &state_index_m = 0, const pset<const Native_Token> &native_tokens_m = {},
                 const pset<const Feature> &immutable_features_m = {}, const pset<const Feature> &features_m = {});
    Alias_Output(const QJsonValue &val);
    Alias_Output(QDataStream &in);
    void serialize(QDataStream &out) const;

    QJsonObject get_Json(void) const;
    void set_id(const c_array &outputid)
    {
        if (alias_id_ == c_array(32, 0))
            alias_id_ = outputid.hash<QCryptographicHash::Blake2b_256>();
    };
    c_array get_id(void) const
    {
        return alias_id_;
    }
    quint32 state_index_, foundry_counter_;
    fl_array<quint16> state_metadata_;

  private:
    Alias_ID alias_id_;
};


}; // namespace qiota
