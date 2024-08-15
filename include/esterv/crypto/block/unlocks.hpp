#pragma once

#include "block/carray.hpp"
#include "block/qsignatures.hpp"
namespace esterv::crypto::block
{
class Unlock : public C_Base<OutputType>
{
  public:
    enum types : quint8
    {
        Signature_typ = 0,
        Reference_typ = 1,
        Alias_typ = 2,
        NFT_typ = 3
    };

    Unlock(types typ);
    template <typename from_type> static std::shared_ptr<const Unlock> from_(from_type &val);

    static std::shared_ptr<const Unlock> Signature(const std::shared_ptr<const class Signature> &signature_m);
    static std::shared_ptr<const Unlock> Reference(const quint16 &reference_m);
    static std::shared_ptr<const Unlock> Alias(const quint16 &alias_reference_unlock_index_m);
    static std::shared_ptr<const Unlock> NFT(const quint16 &nft_reference_unlock_index_m);

    virtual void serialize(QDataStream &out) const;
    virtual QJsonObject get_Json(void) const;

    types type(void) const
    {
        return type_m;
    }

  private:
    const types type_m;
};

class Signature_Unlock : public Unlock
{
  public:
    Signature_Unlock(const std::shared_ptr<const class Signature> &signature_m);
    Signature_Unlock(const QJsonValue &val);
    Signature_Unlock(QDataStream &in);
    void serialize(QDataStream &out) const;
    QJsonObject get_Json(void) const;

  private:
    std::shared_ptr<const class Signature> signature_;
};

class Reference_Unlock : public Unlock
{
  public:
    Reference_Unlock(const quint16 &reference_m);
    Reference_Unlock(const QJsonValue &val);
    Reference_Unlock(QDataStream &in);
    void serialize(QDataStream &out) const;
    QJsonObject get_Json(void) const;

  private:
    quint16 reference_;
};

class Alias_Unlock : public Unlock
{
  public:
    Alias_Unlock(const quint16 &alias_reference_unlock_index_m);
    Alias_Unlock(const QJsonValue &val);
    Alias_Unlock(QDataStream &in);
    void serialize(QDataStream &out) const;
    QJsonObject get_Json(void) const;

  private:
    quint16 alias_reference_unlock_index_;
};

class NFT_Unlock : public Unlock
{
  public:
    NFT_Unlock(const quint16 &nft_reference_unlock_index_m);
    NFT_Unlock(const QJsonValue &val);
    NFT_Unlock(QDataStream &in);
    void serialize(QDataStream &out) const;
    QJsonObject get_Json(void) const;

  private:
    quint16 nft_reference_unlock_index_;
};
}; // namespace qblocks
}; // namespace qiota
