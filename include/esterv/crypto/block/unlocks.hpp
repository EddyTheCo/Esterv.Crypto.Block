#pragma once

#include "esterv/crypto/block/signatures.hpp"
namespace esterv::crypto::block
{
class Unlock : public C_Base<UnlockType>
{
  protected:
    Unlock(UnlockType typ) : C_Base{typ}
    {
    }

  public:
    template <typename from_type> static std::shared_ptr<const Unlock> from(from_type &val);

    [[nodiscard]] static std::shared_ptr<const Unlock> Signature(
        const std::shared_ptr<const class Signature> &signature);
    [[nodiscard]] static std::shared_ptr<const Unlock> Reference(const quint16 &reference);
    [[nodiscard]] static std::shared_ptr<const Unlock> Account(const quint16 &accountReferenceUnlockIndex);
    [[nodiscard]] static std::shared_ptr<const Unlock> Anchor(const quint16 &anchorReferenceUnlockIndex);
    [[nodiscard]] static std::shared_ptr<const Unlock> NFT(const quint16 &nftReferenceUnlockIndex);
    [[nodiscard]] static std::shared_ptr<const Unlock> Multi(const pvector<const Unlock> &unlocks);
    [[nodiscard]] static std::shared_ptr<const Unlock> Empty();
};

class MultiUnlock : public Unlock
{
    pvector<const Unlock> m_unlocks;
    MultiUnlock(const pvector<const Unlock> &unlocks) : Unlock{UnlockType::Multi}, m_unlocks{unlocks}
    {
    }
    MultiUnlock(const QJsonValue &val)
        : Unlock{UnlockType::Multi}, m_unlocks{getTvec<Unlock>(val.toObject()["unlocks"].toArray())}
    {
    }
    MultiUnlock(QDataStream &in) : Unlock{UnlockType::Multi}, m_unlocks{Signature::from<QDataStream>(in)}
    {
    }

  public:
    void serialize(QDataStream &out) const override
    {
        Unlock::serialize(out);
        serializeList<quint8>(out, m_unlocks);
    }
    void addJson(QJsonObject &var) const override
    {
        Unlock::addJson(var);
        if (m_signature)
        {
            QJsonObject signature;
            m_signature->addJson(signature);
            var.insert("signature", signature);
        }
    }
    [[nodiscard]] auto signature(void) const
    {
        return m_signature;
    }
    void setSignature(const std::shared_ptr<const class Signature> &signature)
    {
        if (signature)
        {
            m_signature = signature;
        }
    }
    friend class Unlock;
};

class SignatureUnlock : public Unlock
{
    std::shared_ptr<const class Signature> m_signature;
    SignatureUnlock(const std::shared_ptr<const class Signature> &signature)
        : Unlock{UnlockType::Signature}, m_signature{signature}
    {
    }
    SignatureUnlock(const QJsonValue &val)
        : Unlock{UnlockType::Signature}, m_signature{Signature::from<const QJsonValue>(val.toObject()["signature"])}
    {
    }
    SignatureUnlock(QDataStream &in) : Unlock{UnlockType::Signature}, m_signature{Signature::from<QDataStream>(in)}
    {
    }

  public:
    void serialize(QDataStream &out) const override
    {
        Unlock::serialize(out);
        if (m_signature)
        {
            m_signature->serialize(out);
        }
    }
    void addJson(QJsonObject &var) const override
    {
        Unlock::addJson(var);
        if (m_signature)
        {
            QJsonObject signature;
            m_signature->addJson(signature);
            var.insert("signature", signature);
        }
    }
    [[nodiscard]] auto signature(void) const
    {
        return m_signature;
    }
    void setSignature(const std::shared_ptr<const class Signature> &signature)
    {
        if (signature)
        {
            m_signature = signature;
        }
    }
    friend class Unlock;
};

class ReferenceUnlock : virtual public Unlock
{
    quint16 m_reference;

  protected:
    ReferenceUnlock(const quint16 &reference) : Unlock{UnlockType::Signature}, m_reference{reference}
    {
    }
    ReferenceUnlock(const QJsonValue &val)
        : Unlock{UnlockType::Signature}, m_reference{static_cast<quint16>(val.toObject()["reference"].toInt())}
    {
    }
    ReferenceUnlock(QDataStream &in) : Unlock{UnlockType::Signature}
    {
        in >> m_reference;
    }

  public:
    void serialize(QDataStream &out) const override
    {
        Unlock::serialize(out);
        out << m_reference;
    }
    void addJson(QJsonObject &var) const override
    {
        Unlock::addJson(var);
        var.insert("reference", (int)m_reference);
    }
    [[nodiscard]] auto reference(void) const
    {
        return m_reference;
    }
    void setReference(const quint16 &reference)
    {
        m_reference = reference;
    }
    friend class Unlock;
};

class AccountUnlock : public ReferenceUnlock
{
    AccountUnlock(const quint16 &accountReferenceUnlockIndex)
        : ReferenceUnlock(accountReferenceUnlockIndex), Unlock{UnlockType::Account}
    {
    }
    AccountUnlock(const QJsonValue &val) : ReferenceUnlock(val), Unlock{UnlockType::Account}
    {
    }
    AccountUnlock(QDataStream &in) : ReferenceUnlock(in), Unlock{UnlockType::Account}
    {
    }

    friend class Unlock;
};
class AnchorUnlock : public ReferenceUnlock
{
    AnchorUnlock(const quint16 &anchorReferenceUnlockIndex)
        : ReferenceUnlock(anchorReferenceUnlockIndex), Unlock{UnlockType::Anchor}
    {
    }
    AnchorUnlock(const QJsonValue &val) : ReferenceUnlock(val), Unlock{UnlockType::Anchor}
    {
    }
    AnchorUnlock(QDataStream &in) : ReferenceUnlock(in), Unlock{UnlockType::Anchor}
    {
    }

    friend class Unlock;
};

class NFTUnlock : public ReferenceUnlock
{
    NFTUnlock(const quint16 &nftReferenceUnlockIndex)
        : ReferenceUnlock(nftReferenceUnlockIndex), Unlock{UnlockType::NFT}
    {
    }
    NFTUnlock(const QJsonValue &val) : ReferenceUnlock(val), Unlock{UnlockType::NFT}
    {
    }
    NFTUnlock(QDataStream &in) : ReferenceUnlock(in), Unlock{UnlockType::NFT}
    {
    }

    friend class Unlock;
};
class EmptyUnlock : public Unlock
{
    EmptyUnlock() : Unlock{UnlockType::Empty}
    {
    }
    friend class Unlock;
};

}; // namespace esterv::crypto::block
