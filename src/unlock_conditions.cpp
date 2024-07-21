#include "esterv/crypto/block/unlock_conditions.hpp"

namespace esterv::crypto::block
{

const QHash<UnlockConditionType, QString> AddressUnlockCondition::jsonStr{
    {UnlockConditionType::Address, "address"},          {UnlockConditionType::StorageDepositReturn, "returnAddress"},
    {UnlockConditionType::Expiration, "returnAddress"}, {UnlockConditionType::StateControllerAddress, "address"},
    {UnlockConditionType::GovernorAddress, "address"},  {UnlockConditionType::ImmutableAccountAddress, "address"}};
template <class from_type> std::shared_ptr<const UnlockCondition> UnlockCondition::from(from_type &val)
{
    const auto type = getType<UnlockConditionType>(val);

    switch (type)
    {
    case UnlockConditionType::Address:
        return std::shared_ptr<const UnlockCondition>(new AddressUnlockCondition(val));
    case UnlockConditionType::StorageDepositReturn:
        return std::shared_ptr<const UnlockCondition>(new StorageDepositReturnUnlockCondition(val));
    case UnlockConditionType::Timelock:
        return std::shared_ptr<const UnlockCondition>(new TimelockUnlockCondition(val));
    case UnlockConditionType::Expiration:
        return std::shared_ptr<const UnlockCondition>(new ExpirationUnlockCondition(val));
    case UnlockConditionType::StateControllerAddress:
        return std::shared_ptr<const UnlockCondition>(new StateControllerAddressUnlockCondition(val));
    case UnlockConditionType::GovernorAddress:
        return std::shared_ptr<const UnlockCondition>(new GovernorAddressUnlockCondition(val));
    case UnlockConditionType::ImmutableAccountAddress:
        return std::shared_ptr<const UnlockCondition>(new ImmutableAccountAddressUnlockCondition(val));
    default:
        return nullptr;
    }
}

template std::shared_ptr<const UnlockCondition> UnlockCondition::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const UnlockCondition> UnlockCondition::from<QDataStream>(QDataStream &val);

std::shared_ptr<const UnlockCondition> UnlockCondition::Address(const std::shared_ptr<const class Address> &address)
{
    return std::shared_ptr<const UnlockCondition>(new AddressUnlockCondition(address));
}

std::shared_ptr<const UnlockCondition> UnlockCondition::StorageDepositReturn(
    const std::shared_ptr<const class Address> &returnAddress, const quint64 &returnAmount)
{
    return std::shared_ptr<const UnlockCondition>(new StorageDepositReturnUnlockCondition(returnAddress, returnAmount));
}
std::shared_ptr<const UnlockCondition> UnlockCondition::Timelock(const quint32 &slotIndex)
{
    return std::shared_ptr<const UnlockCondition>(new TimelockUnlockCondition(slotIndex));
}
std::shared_ptr<const UnlockCondition> UnlockCondition::Expiration(
    const std::shared_ptr<const class Address> &returnAddress, const quint32 &slotIndex)
{
    return std::shared_ptr<const UnlockCondition>(new ExpirationUnlockCondition(returnAddress, slotIndex));
}
std::shared_ptr<const UnlockCondition> UnlockCondition::StateControllerAddress(
    const std::shared_ptr<const class Address> &address)
{
    return std::shared_ptr<const UnlockCondition>(new StateControllerAddressUnlockCondition(address));
}

std::shared_ptr<const UnlockCondition> UnlockCondition::GovernorAddress(
    const std::shared_ptr<const class Address> &address)
{
    return std::shared_ptr<UnlockCondition>(new GovernorAddressUnlockCondition(address));
}

std::shared_ptr<const UnlockCondition> UnlockCondition::ImmutableAccountAddress(
    const std::shared_ptr<const class Address> &address)
{
    return std::shared_ptr<UnlockCondition>(new ImmutableAccountAddressUnlockCondition(address));
}

} // namespace qiota
