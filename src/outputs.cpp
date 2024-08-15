#include "esterv/crypto/block/outputs.hpp"
namespace esterv::crypto::block
{

const QHash<OutputType, QString> ChainOutput::jsonStr{{OutputType::NFT, "nftId"},
                                                      {OutputType::Account, "accountId"},
                                                      {OutputType::Anchor, "anchorId"},
                                                      {OutputType::Delegation, "delegationId"}};
std::shared_ptr<Output> Output::clone(void) const
{
    QByteArray var;
    auto wBuffer = QDataStream(&var, QIODevice::WriteOnly | QIODevice::Append);
    wBuffer.setByteOrder(QDataStream::LittleEndian);
    serialize(wBuffer);
    auto rBuffer = QDataStream(&var, QIODevice::ReadOnly);
    rBuffer.setByteOrder(QDataStream::LittleEndian);
    return from<QDataStream>(rBuffer);
}

const QHash<OutputType, QString> Output::typesStr = {{OutputType::Basic, "/basic"},
                                                     {OutputType::NFT, "/nft"},
                                                     {OutputType::Foundry, "/foundry"},
                                                     {OutputType::Anchor, "/Anchor"},
                                                     {OutputType::Account, "/Account"}};

template <class from_type> std::shared_ptr<Output> Output::from(from_type &val)
{
    const auto type = getType<OutputType>(val);
    switch (type)
    {
    case OutputType::Basic:
        return std::shared_ptr<Output>(new BasicOutput(val));
    case OutputType::NFT:
        return std::shared_ptr<Output>(new NFTOutput(val));
    case OutputType::Foundry:
        return std::shared_ptr<Output>(new FoundryOutput(val));
    case OutputType::Anchor:
        return std::shared_ptr<Output>(new AnchorOutput(val));
    case OutputType::Account:
        return std::shared_ptr<Output>(new AccountOutput(val));
    case OutputType::Delegation:
        return std::shared_ptr<Output>(new DelegationOutput(val));
    default:
        return nullptr;
    }
}
template std::shared_ptr<Output> Output::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<Output> Output::from<QDataStream>(QDataStream &val);

quint64 Output::minDepositOfOutput(const quint64 &wkey, const quint64 &wdata, const quint64 &v_byte_cost) const
{
    quint64 offset = 34 * wkey + (32 + 4 + 4) * wdata;
    c_array serializedOutput;
    auto buffer = QDataStream(&serializedOutput, QIODevice::WriteOnly | QIODevice::Append);
    buffer.setByteOrder(QDataStream::LittleEndian);
    this->serialize(buffer);
    quint64 outp = serializedOutput.size() * wdata;
    return (outp + offset) * v_byte_cost;
}

std::shared_ptr<Output> Output::Basic(const quint64 &amount, const quint64 &mana,
                                      const pset<const UnlockCondition> &unlockConditions,
                                      const pset<const Feature> &features)
{
    return std::shared_ptr<Output>{new BasicOutput(mana, features, amount, unlockConditions)};
}
std::shared_ptr<Output> Output::NFT(const quint64 &amount, const quint64 &mana,
                                    const pset<const UnlockCondition> &unlockConditions,
                                    const pset<const Feature> &features, const pset<const Feature> &immutableFeatures)
{
    return std::shared_ptr<Output>(new NFTOutput(immutableFeatures, mana, features, amount, unlockConditions));
}
std::shared_ptr<Output> Output::Foundry(const quint64 &amount, const quint32 &serialNumber,
                                        const std::shared_ptr<const TokenScheme> &tokenScheme,
                                        const pset<const UnlockCondition> &unlockConditions,
                                        const pset<const Feature> &features,
                                        const pset<const Feature> &immutableFeatures)
{
    return std::shared_ptr<Output>(new FoundryOutput(tokenScheme, serialNumber, immutableFeatures, /*mana*/ quint64(0),
                                                     features, amount, unlockConditions));
}
std::shared_ptr<Output> Output::Account(const quint64 &amount, const quint64 &mana, const quint32 &foundryCounter,
                                        const pset<const UnlockCondition> &unlockConditions,
                                        const pset<const Feature> &features,
                                        const pset<const Feature> &immutableFeatures)
{
    return std::shared_ptr<Output>(
        new AccountOutput(foundryCounter, immutableFeatures, mana, features, amount, unlockConditions));
}
std::shared_ptr<Output> Output::Anchor(const quint64 &amount, const quint64 &mana, const quint32 &stateIndex,
                                       const pset<const UnlockCondition> &unlockConditions,
                                       const pset<const Feature> &features,
                                       const pset<const Feature> &immutableFeatures)
{
    return std::shared_ptr<Output>(
        new AnchorOutput(stateIndex, immutableFeatures, mana, features, amount, unlockConditions));
}
std::shared_ptr<Output> Output::Delegation(const quint64 &amount, const quint64 &delegatedAmount,
                                           const std::shared_ptr<const Address> &validatorAddress,
                                           const quint64 &startEpoch, const quint64 &endEpoch,
                                           const pset<const UnlockCondition> &unlockConditions)
{
    return std::shared_ptr<Output>(new DelegationOutput(validatorAddress, startEpoch, endEpoch, pset<const Feature>{},
                                                        delegatedAmount, pset<const Feature>{}, amount,
                                                        unlockConditions));
}

}; // namespace qblocks

