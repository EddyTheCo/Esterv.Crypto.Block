#pragma once

#include "esterv/crypto/block/addresses.hpp"
#include "esterv/crypto/block/carray.hpp"

namespace esterv::crypto::block
{
class UnlockCondition : public C_Base<UnlockConditionType>
{
  protected:
    UnlockCondition(UnlockConditionType typ) : C_Base<UnlockConditionType>(typ)
    {
    }

  public:
    template <class from_type> static std::shared_ptr<const UnlockCondition> from(from_type &val);

    [[nodiscard]] static std::shared_ptr<const UnlockCondition> Address(
        const std::shared_ptr<const class Address> &address = nullptr);

    [[nodiscard]] static std::shared_ptr<const UnlockCondition> StorageDepositReturn(
        const std::shared_ptr<const class Address> &returnAddress = nullptr, const quint64 &returnAmount = 0);

    [[nodiscard]] static std::shared_ptr<const UnlockCondition> Timelock(const quint32 &slotIndex = 0);

    [[nodiscard]] static std::shared_ptr<const UnlockCondition> Expiration(
        const std::shared_ptr<const class Address> &returnAddress = nullptr, const quint32 &slotIndex = 0);

    [[nodiscard]] static std::shared_ptr<const UnlockCondition> StateControllerAddress(
        const std::shared_ptr<const class Address> &address = nullptr);

    [[nodiscard]] static std::shared_ptr<const UnlockCondition> GovernorAddress(
        const std::shared_ptr<const class Address> &address);

    [[nodiscard]] static std::shared_ptr<const UnlockCondition> ImmutableAccountAddress(
        const std::shared_ptr<const class Address> &address);
};

class AddressUnlockCondition : virtual public UnlockCondition
{
  protected:
    AddressUnlockCondition(const std::shared_ptr<const class Address> &address)
        : UnlockCondition(UnlockConditionType::Address), m_address{address}
    {
    }
    AddressUnlockCondition(const QJsonValue &val)
        : AddressUnlockCondition(Address::from<const QJsonValue>(val.toObject()[jsonStr[m_type]]))
    {
    }
    AddressUnlockCondition(QDataStream &in) : AddressUnlockCondition(Address::from<QDataStream>(in))
    {
    }

    std::shared_ptr<const class Address> m_address;
    static const QHash<UnlockConditionType, QString> jsonStr;

  public:
    void serialize(QDataStream &out) const override
    {
        if (m_type == UnlockConditionType::Address)
            UnlockCondition::serialize(out);
        m_address->serialize(out);
    }
    void addJson(QJsonObject &var) const override
    {
        if (m_type == UnlockConditionType::Address)
            UnlockCondition::addJson(var);

        QJsonObject address;
        m_address->addJson(address);
        var.insert(jsonStr[m_type], address);
    }
    [[nodiscard]] auto address(void) const
    {
        return m_address;
    }
    void setAddress(std::shared_ptr<const class Address> address)
    {
        m_address = address;
    }
    friend class UnlockCondition;
};

class StorageDepositReturnUnlockCondition : public AddressUnlockCondition
{
    quint64 m_returnAmount;
    StorageDepositReturnUnlockCondition(const std::shared_ptr<const class Address> &returnAddress,
                                        const quint64 &returnAmount)
        : AddressUnlockCondition(returnAddress), UnlockCondition(UnlockConditionType::StorageDepositReturn),
          m_returnAmount(returnAmount)
    {
    }
    StorageDepositReturnUnlockCondition(const QJsonValue &val)
        : AddressUnlockCondition(val), UnlockCondition(UnlockConditionType::StorageDepositReturn),
          m_returnAmount{val.toObject()["amount"].toString().toULongLong()}
    {
    }
    StorageDepositReturnUnlockCondition(QDataStream &in)
        : AddressUnlockCondition(in), UnlockCondition(UnlockConditionType::StorageDepositReturn)
    {
        in >> m_returnAmount;
    }
    void serialize(QDataStream &out) const override
    {
        if (m_type == UnlockConditionType::StorageDepositReturn)
            AddressUnlockCondition::serialize(out);
        out << m_returnAmount;
    }
    void addJson(QJsonObject &var) const override
    {
        if (m_type == UnlockConditionType::StorageDepositReturn)
            AddressUnlockCondition::addJson(var);
        var.insert("amount", QString::number(m_returnAmount));
    }

  public:
    [[nodiscard]] auto returnAmount(void) const
    {
        return m_returnAmount;
    }
    void setReturnAmount(quint64 returnAmount)
    {
        m_returnAmount = returnAmount;
    }

    friend class UnlockCondition;
};

class TimelockUnlockCondition : virtual public UnlockCondition
{
  protected:
    quint32 m_slotIndex;
    TimelockUnlockCondition(const quint32 &slotIndex)
        : UnlockCondition(UnlockConditionType ::Timelock), m_slotIndex(slotIndex)
    {
    }
    TimelockUnlockCondition(const QJsonValue &val) : TimelockUnlockCondition(val.toObject()["slot"].toInteger())
    {
    }
    TimelockUnlockCondition(QDataStream &in) : UnlockCondition(UnlockConditionType::Timelock)
    {
        in >> m_slotIndex;
    }

  public:
    void serialize(QDataStream &out) const override
    {
        if (m_type == UnlockConditionType::Timelock)
            UnlockCondition::serialize(out);
        out << m_slotIndex;
    }
    void addJson(QJsonObject &var) const override
    {
        if (m_type == UnlockConditionType::Timelock)
            UnlockCondition::addJson(var);
        var.insert("slot", (int)m_slotIndex);
    }
    [[nodiscard]] auto slotIndex(void) const
    {
        return m_slotIndex;
    }
    void setSlotIndex(quint32 slotIndex)
    {
        m_slotIndex = slotIndex;
    }

    friend class UnlockCondition;
};

class ExpirationUnlockCondition : public AddressUnlockCondition, public TimelockUnlockCondition
{
    ExpirationUnlockCondition(const std::shared_ptr<const class Address> &returnAddress, const quint32 &slotIndex)
        : AddressUnlockCondition(returnAddress), TimelockUnlockCondition(slotIndex),
          UnlockCondition(UnlockConditionType::Expiration)
    {
    }
    ExpirationUnlockCondition(const QJsonValue &val)
        : AddressUnlockCondition(val), TimelockUnlockCondition(val), UnlockCondition(UnlockConditionType::Expiration)
    {
    }
    ExpirationUnlockCondition(QDataStream &in)
        : AddressUnlockCondition(in), TimelockUnlockCondition(in), UnlockCondition(UnlockConditionType::Expiration)
    {
    }

  public:
    void serialize(QDataStream &out) const override
    {
        AddressUnlockCondition::serialize(out);
        TimelockUnlockCondition::serialize(out);
    }
    void addJson(QJsonObject &var) const override
    {
        AddressUnlockCondition::addJson(var);
        TimelockUnlockCondition::addJson(var);
    }

    friend class UnlockCondition;
};

class StateControllerAddressUnlockCondition : public AddressUnlockCondition
{
    StateControllerAddressUnlockCondition(const std::shared_ptr<const class Address> &address)
        : AddressUnlockCondition(address), UnlockCondition(UnlockConditionType::StateControllerAddress)
    {
    }
    StateControllerAddressUnlockCondition(const QJsonValue &val)
        : AddressUnlockCondition(val), UnlockCondition(UnlockConditionType::StateControllerAddress)
    {
    }
    StateControllerAddressUnlockCondition(QDataStream &in)
        : AddressUnlockCondition(in), UnlockCondition(UnlockConditionType::StateControllerAddress)
    {
    }

    friend class UnlockCondition;
};

class GovernorAddressUnlockCondition : public AddressUnlockCondition
{
    GovernorAddressUnlockCondition(const std::shared_ptr<const class Address> &address)
        : AddressUnlockCondition(address), UnlockCondition(UnlockConditionType::GovernorAddress)
    {
    }
    GovernorAddressUnlockCondition(const QJsonValue &val)
        : AddressUnlockCondition(val), UnlockCondition(UnlockConditionType::GovernorAddress)
    {
    }
    GovernorAddressUnlockCondition(QDataStream &in)
        : AddressUnlockCondition(in), UnlockCondition(UnlockConditionType::GovernorAddress)
    {
    }

    friend class UnlockCondition;
};
class ImmutableAccountAddressUnlockCondition : public AddressUnlockCondition
{
    ImmutableAccountAddressUnlockCondition(const std::shared_ptr<const class Address> &address)
        : AddressUnlockCondition(address), UnlockCondition(UnlockConditionType::ImmutableAccountAddress)
    {
    }
    ImmutableAccountAddressUnlockCondition(const QJsonValue &val)
        : AddressUnlockCondition(val), UnlockCondition(UnlockConditionType::ImmutableAccountAddress)
    {
    }
    ImmutableAccountAddressUnlockCondition(QDataStream &in)
        : AddressUnlockCondition(in), UnlockCondition(UnlockConditionType::ImmutableAccountAddress)
    {
    }

    friend class UnlockCondition;
};
} // namespace qiota::qblocks
