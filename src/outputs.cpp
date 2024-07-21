#include "block/qoutputs.hpp"
namespace qiota
{
namespace qblocks
{

c_array Output::getId(void) const
{
    return c_array(ByteSizes::hash, 0);
}
const QHash<OutputType, QString> Output::typesstr = {
    {Basic, "/basic"}, {NFT, "/nft"}, {Foundry, "/foundry"}, {Anchor, "/Anchor"}, {Account, "/Account"}};

void Output::serialize(QDataStream &out) const
{
    out << type() << m_amount;
    serializeList<quint8>(out, m_unlockConditions);
    serializeList<quint8>(out, m_features);
}
void Output::setId(const c_array &id)
{
}
void Output::consume(void)
{
    m_features.clear();
    m_unlockConditions.clear();
}
void FoundryOutput::consume(void)
{
    m_features.clear();
}
QJsonObject Output::getJson(void) const
{
    QJsonObject var;
    var.insert("amount", QString::number(m_amount));

    QJsonArray unarr;
    for (const auto &v : m_unlockConditions)
        unarr.push_back(v->get_Json());
    var.insert("unlockConditions", unarr);
    if (m_features.size())
    {
        QJsonArray fearr;
        for (const auto &v : m_features)
            fearr.push_back(v->get_Json());
        var.insert("features", fearr);
    }
    return var;
}
Output::Output(OutputType typ, const quint64 &amount, const pset<const Unlock_Condition> &unlockConditions,
               const pset<const Feature> &features)
    : C_Base<OutputType>{typ}, m_amount{amount}, m_unlockConditions{unlockConditions}, m_features{features} {
}
template <class from_type> std::shared_ptr<Output> Output::from(from_type &val)
{
    const auto type_ = get_type<types>(val);
    switch (type_)
    {
    case Basic_typ:
        return std::shared_ptr<Output>(new BasicOutput(val));
    case NFT_typ:
        return std::shared_ptr<Output>(new NFTOutput(val));
    case Foundry_typ:
        return std::shared_ptr<Output>(new FoundryOutput(val));
    case Anchor_typ:
        return std::shared_ptr<Output>(new AnchorOutput(val));
    case Account_typ:
        return std::shared_ptr<Output>(new AccountOutput(val));
    default:
        return nullptr;
    }
}
template std::shared_ptr<Output> Output::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<Output> Output::from<QDataStream>(QDataStream &val);
template std::shared_ptr<Output> Output::from<const QJsonValueRef>(const QJsonValueRef &val);
template std::shared_ptr<Output> Output::from<QJsonValueConstRef const>(QJsonValueConstRef const &);

quint64 Output::minDepositOfOutput(const quint64 &wkey, const quint64 &wdata, const quint64 &v_byte_cost) const
{
    quint64 offset = 34 * wkey + (32 + 4 + 4) * wdata;
    c_array serializedoutput;
    serializedoutput.fromObject<Output>(*this);
    quint64 outp = serializedoutput.size() * wdata;
    return (outp + offset) * v_byte_cost;
}

Output::Output(OutputType typ, const QJsonValue &val)
    : Output(typ, val.toObject()["amount"].toString().toULongLong(),
             get_T<Unlock_Condition>(val.toObject()["unlockConditions"].toArray()),
             get_T<Feature>(val.toObject()["features"].toArray()))
{
}

Output::Output(OutputType typ, QDataStream &in) : C_Base<OutputType>{typ}
{
    in >> m_amount;
    m_unlockConditions = deserializeList<quint8, UnlockCondition>(in);
    m_features = deserializeList<quint8, Feature>(in);
}

template <class... Args> auto Output::Basic(Args &&...args)
{
    return std::shared_ptr<Output>{new BasicOutput(std::forward<Args>(args)...)};
}

template <class... Args> auto Output::NFT(Args &&...args)
{
    return std::shared_ptr<Output>(new NFTOutput(std::forward<Args>(args)...));
}

NFTOutput::NFTOutput(const QJsonValue &val)
    : NFTOutput(val.toObject()["mana"].toString().toULongLong(),
                get_T<Feature>(val.toObject()["immutableFeatures"].toArray()), val),
      m_nftId(NFTID(val.toObject()["nftId"]))
{
}

NFTOutput::NFTOutput(QDataStream &in) : Output(types::NFT_typ)
{
    in >> m_amount;
    m_nftId = NFTID(ByteSizes::hash, 0); // Check why initialize to zeroes here
    in >> m_nftId;
    m_unlockConditions = deserializeList<quint8, Unlock_Condition>(in);
    m_features = deserializeList<quint8, Feature>(in);
    m_immutableFeatures = deserializeList<quint8, Feature>(in);
}

QJsonObject NFTOutput::getJson(void) const
{
    auto var = this->Output::getJson();
    var.insert("nftId", m_nftId.toHexString());
    return var;
}
void NFTOutput::serialize(QDataStream &out) const
{
    out << type() << m_amount;
    out << m_nftId;
    serializeList<quint8>(out, m_unlockConditions);
    serializeList<quint8>(out, m_features);
    serializeList<quint8>(out, m_immutableFeatures);
}
template <class... Args> auto Output::Foundry(Args &&...args)
{
    return std::shared_ptr<Output>(new FoundryOutput(std::forward<Args>(args)...));
}
template <class... Args>
FoundryOutput::FoundryOutput(const std::shared_ptr<Token_Scheme> &tokenScheme, const quint32 &serialNumber,
                             Args &&...args)
    : Output(types::Foundry_typ, std::forward<Args>(args)...), m_tokenScheme{tokenScheme},
      m_serialNumber{serialNumber} {};
FoundryOutput::FoundryOutput(const QJsonValue &val)
    : Output(types::Foundry_typ, val),
      m_tokenScheme(TokenScheme::from_<const QJsonValue>(val.toObject()["tokenScheme"])),
      m_serialNumber(val.toObject()["serialNumber"].toInteger())
{
}

FoundryOutput::FoundryOutput(QDataStream &in) : Output(types::Foundry_typ)
{
    in >> amount_;
    native_tokens_ = deserializeList<quint8, Native_Token>(in);
    in >> serial_number_;
    token_scheme_ = Token_Scheme::from_<QDataStream>(in);
    unlock_conditions_ = deserializeList<quint8, Unlock_Condition>(in);
    features_ = deserializeList<quint8, Feature>(in);
    immutable_features_ = deserializeList<quint8, Feature>(in);
}

QJsonObject FoundryOutput::getJson(void) const
{
    auto var = this->Output::getJson();
    var.insert("serialNumber", (int)serial_number_);
    var.insert("tokenScheme", token_scheme_->get_Json());
    return var;
}
void FoundryOutput::serialize(QDataStream &out) const
{

    out << type() << amount_;
    serializeList<quint8>(out, native_tokens_);
    out << serial_number_;
    token_scheme_->serialize(out);
    serializeList<quint8>(out, unlock_conditions_);
    serializeList<quint8>(out, features_);
    serializeList<quint8>(out, immutable_features_);
}

std::shared_ptr<Output> Output::Alias(const quint64 &amount_m, const pset<const Unlock_Condition> &unlock_conditions_m,
                                      const fl_array<quint16> &state_metadata_m, const quint32 &foundry_counter_m,
                                      const quint32 &state_index_m, const pset<const Native_Token> &native_tokens_m,
                                      const pset<const Feature> &immutable_features_m,
                                      const pset<const Feature> &features_m)
{
    return std::shared_ptr<Output>(new Alias_Output(amount_m, unlock_conditions_m, state_metadata_m, foundry_counter_m,
                                                    state_index_m, native_tokens_m, immutable_features_m, features_m));
}

Alias_Output::Alias_Output(const quint64 &amount_m, const pset<const Unlock_Condition> &unlock_conditions_m,
                           const fl_array<quint16> &state_metadata_m, const quint32 &foundry_counter_m,
                           const quint32 &state_index_m, const pset<const Native_Token> &native_tokens_m,
                           const pset<const Feature> &immutable_features_m, const pset<const Feature> &features_m)
    : Output(types::Alias_typ, amount_m, unlock_conditions_m, features_m, native_tokens_m, immutable_features_m),
      alias_id_(Alias_ID(32, 0)), state_index_(state_index_m), foundry_counter_(foundry_counter_m),
      state_metadata_(state_metadata_m){};

Alias_Output::Alias_Output(const QJsonValue &val)
    : Output(types::Alias_typ, val), alias_id_(Alias_ID(val.toObject()["aliasId"])),
      state_index_(val.toObject()["stateIndex"].toInteger()),
      foundry_counter_(val.toObject()["foundryCounter"].toInteger()),
      state_metadata_(fl_array<quint16>(val.toObject()["stateMetadata"]))
{
}

Alias_Output::Alias_Output(QDataStream &in) : Output(types::Alias_typ)
{
    in >> amount_;
    native_tokens_ = deserializeList<quint8, Native_Token>(in);
    alias_id_ = Alias_ID(32, 0);
    in >> alias_id_;
    in >> state_index_ >> state_metadata_ >> foundry_counter_;
    unlock_conditions_ = deserializeList<quint8, Unlock_Condition>(in);
    features_ = deserializeList<quint8, Feature>(in);
    immutable_features_ = deserializeList<quint8, Feature>(in);
}

QJsonObject Alias_Output::get_Json(void) const
{
    auto var = this->Output::get_Json();

    var.insert("aliasId", alias_id_.toHexString());
    var.insert("stateIndex", (int)state_index_);
    var.insert("foundryCounter", (int)foundry_counter_);
    var.insert("stateMetadata", state_metadata_.toHexString());
    return var;
}
void Alias_Output::serialize(QDataStream &out) const
{

    out << type() << amount_;
    serializeList<quint8>(out, native_tokens_);
    out << alias_id_ << state_index_ << state_metadata_ << foundry_counter_;
    serializeList<quint8>(out, unlock_conditions_);
    serializeList<quint8>(out, features_);
    serializeList<quint8>(out, immutable_features_);
}

}; // namespace qblocks

}; // namespace qiota
