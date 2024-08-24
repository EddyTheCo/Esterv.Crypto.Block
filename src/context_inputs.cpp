#include "esterv/crypto/block/context_inputs.hpp"

namespace esterv::crypto::block
{

template <class from_type> std::shared_ptr<const ContextInput> ContextInput::from(from_type &val)
{
    const auto type = getType<ContextInputType>(val);

    switch (type)
    {

    case ContextInputType::BlockIssuanceCredit:
        return std::shared_ptr<const ContextInput>(new BlockIssuanceCreditContextInput(val));
        break;
    case ContextInputType::Commitment:
        return std::shared_ptr<const ContextInput>(new CommitmentContextInput(val));
        break;
    case ContextInputType::Reward:
        return std::shared_ptr<const ContextInput>(new RewardContextInput(val));
        break;
    default:
        return nullptr;
    }
}
template std::shared_ptr<const ContextInput> ContextInput::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const ContextInput> ContextInput::from<QDataStream>(QDataStream &val);

std::shared_ptr<const ContextInput> ContextInput::Commitment(const ID &commitmentId)
{
    return std::shared_ptr<const ContextInput>(new CommitmentContextInput(commitmentId));
}
std::shared_ptr<const ContextInput> ContextInput::BlockIssuanceCredit(const ID &accountId)
{
    return std::shared_ptr<const ContextInput>(new BlockIssuanceCreditContextInput(accountId));
}
std::shared_ptr<const ContextInput> ContextInput::Reward(const quint16 &index)
{
    return std::shared_ptr<const ContextInput>(new RewardContextInput(index));
}

}; // namespace qblocks
