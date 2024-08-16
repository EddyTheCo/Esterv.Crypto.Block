#pragma once

#include "esterv/crypto/block/carray.hpp"

namespace esterv::crypto::block
{
class Signature : public C_Base<SignatureType>
{
  protected:
    Signature(SignatureType typ) : C_Base{typ}
    {
    }

  public:
    template <class from_type> static std::shared_ptr<const Signature> from(from_type &val);

    [[nodiscard]] static std::shared_ptr<const Signature> Ed25519(const c_array &publicKey, const c_array &signature);
};

class Ed25519Signature : public Signature
{
    c_array m_publicKey;
    c_array m_signature;

    Ed25519Signature(const c_array &publicKey, const c_array &signature)
        : Signature{SignatureType::Ed25519}, m_publicKey{publicKey}, m_signature{signature}
    {
    }
    Ed25519Signature(const QJsonValue &val)
        : Signature{SignatureType::Ed25519}, m_publicKey{val.toObject()["publicKey"]},
          m_signature{val.toObject()["signature"]}
    {
    }
    Ed25519Signature(QDataStream &in) : Signature{SignatureType::Ed25519}
    {
        m_publicKey = c_array(ByteSizes::hash, 0);
        in >> m_publicKey;
        m_signature = c_array(ByteSizes::hash, 0);
        in >> m_signature;
    }

  public:
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out << m_publicKey;
        out << m_signature;
    }
    void addJson(QJsonObject &var) const override
    {
        C_Base::addJson(var);
        var.insert("publicKey", m_publicKey.toHexString());
        var.insert("signature", m_signature.toHexString());
    }
    [[nodiscard]] auto publicKey(void) const
    {
        return m_publicKey;
    }
    [[nodiscard]] auto signature(void) const
    {
        return m_signature;
    }
    void setSignature(const c_array &signature)
    {
        m_signature = signature;
    }
    void setPublicKey(const c_array &publicKey)
    {
        m_publicKey = publicKey;
    }

    friend class Signature;
};

}; // namespace esterv::crypto::block
