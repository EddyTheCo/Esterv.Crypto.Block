#pragma once

#include "esterv/crypto/block/addresses.hpp"

namespace esterv::crypto::block
{

class MetadataEntries : public std::map<fl_array<quint8>, fl_array<quint16>>
{
  public:
    MetadataEntries(const QJsonValue &val);
    MetadataEntries(QDataStream &in);
    friend QDataStream &operator<<(QDataStream &out, const MetadataEntries &obj)
    {
        serializeList<quint8>(out, obj);
        return out;
    }
    [[nodiscard]] QJsonArray getJson() const;
};

class Feature : public C_Base<FeatureType>
{
  protected:
    Feature(FeatureType typ);

  public:
    template <class from_type> static std::shared_ptr<const Feature> from(from_type &val);

    [[nodiscard]] static std::shared_ptr<const Feature> Sender(const std::shared_ptr<const Address> &sender);
    [[nodiscard]] static std::shared_ptr<const Feature> Issuer(const std::shared_ptr<const Address> &issuer);
    [[nodiscard]] static std::shared_ptr<const Feature> Metadata(const MetadataEntries &entries);
    [[nodiscard]] static std::shared_ptr<const Feature> Tag(const tagF &tag);
    [[nodiscard]] static std::shared_ptr<const Feature> NativeToken(const TokenID &tokenId, const uint256 &amount);
};

class SenderFeature : public Feature
{
    std::shared_ptr<const Address> m_sender;

    SenderFeature(const std::shared_ptr<const Address> &sender) : Feature(FeatureType::Sender), m_sender{sender}
    {
    }
    SenderFeature(const QJsonValue &val)
        : Feature{FeatureType::Sender}, m_sender{Address::from<const QJsonValue>(val.toObject()["address"])}
    {
    }
    SenderFeature(QDataStream &in) : Feature{FeatureType::Sender}, m_sender{Address::from<QDataStream>(in)}
    {
    }

  public:
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        m_sender->serialize(out);
    }
    void addJson(QJsonObject &var) const override
    {
        Feature::addJson(var);
        QJsonObject address;
        m_sender->addJson(address);
        var.insert("address", address);
    }
    auto sender(void) const
    {
        return m_sender;
    }
    void setSender(const std::shared_ptr<const Address> &sender)
    {
        m_sender = sender;
    }
    friend class Feature;
};
class IssuerFeature : public Feature
{
    std::shared_ptr<const Address> m_issuer;

    IssuerFeature(const std::shared_ptr<const Address> &issuer) : Feature(FeatureType::Issuer), m_issuer{issuer}
    {
    }
    IssuerFeature(const QJsonValue &val)
        : Feature(FeatureType::Issuer), m_issuer{Address::from<const QJsonValue>(val.toObject()["address"])}
    {
    }
    IssuerFeature(QDataStream &in) : Feature(FeatureType::Issuer), m_issuer{Address::from<QDataStream>(in)}
    {
    }

  public:
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        m_issuer->serialize(out);
    }
    void addJson(QJsonObject &var) const override
    {
        Feature::addJson(var);
        QJsonObject address;
        m_issuer->addJson(address);
        var.insert("address", address);
    }
    [[nodiscard]] auto issuer(void) const
    {
        return m_issuer;
    }
    void setIssuer(const std::shared_ptr<const Address> &issuer)
    {
        m_issuer = issuer;
    }
    friend class Feature;
};

class MetadataFeature : public Feature
{
    MetadataEntries m_entries;
    MetadataFeature(const MetadataEntries &entries) : Feature(FeatureType::Metadata), m_entries{entries}
    {
    }

    MetadataFeature(const QJsonValue &val)
        : Feature(FeatureType::Metadata), m_entries{MetadataEntries(val.toObject()["entries"])}
    {
    }
    MetadataFeature(QDataStream &in) : Feature(FeatureType::Metadata), m_entries{in}
    {
    }

  public:
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out << m_entries;
    }
    void addJson(QJsonObject &var) const override
    {
        Feature::addJson(var);
        var.insert("entries", m_entries.getJson());
    }

    [[nodiscard]] auto entries(void) const
    {
        return m_entries;
    }
    void setEntries(const MetadataEntries &entries)
    {
        m_entries = entries;
    }
    friend class Feature;
};

class TagFeature : public Feature
{
    fl_array<quint8> m_tag;
    TagFeature(const fl_array<quint8> &tag) : Feature(FeatureType::Tag), m_tag{tag}
    {
    }
    TagFeature(const QJsonValue &val) : Feature(FeatureType::Tag), m_tag{fl_array<quint8>(val.toObject()["tag"])}
    {
    }
    TagFeature(QDataStream &in) : Feature(FeatureType::Tag)
    {
        in >> m_tag;
    }

  public:
    void serialize(QDataStream &out) const override
    {
        Feature::serialize(out);
        out << m_tag;
    }
    void addJson(QJsonObject &var) const override
    {
        Feature::addJson(var);
        var.insert("tag", m_tag.toHexString());
    }

    [[nodiscard]] auto tag(void) const
    {
        return m_tag;
    }
    void setTag(const fl_array<quint8> &tag)
    {
        m_tag = tag;
    }
    friend class Feature;
};

class NativeTokenFeature : public Feature
{
    TokenID m_tokenId;
    uint256 m_amount;
    NativeTokenFeature(const TokenID &tokenId, const uint256 &amount)
        : Feature(FeatureType::NativeToken), m_tokenId{tokenId}, m_amount{amount}
    {
    }
    NativeTokenFeature(const QJsonValue &val)
        : Feature(FeatureType::NativeToken), m_tokenId{TokenID(val.toObject()["id"])},
          m_amount{uint256(val.toObject()["amount"].toString())}
    {
    }
    NativeTokenFeature(QDataStream &in) : Feature(FeatureType::NativeToken)
    {
        m_tokenId = c_array(ByteSizes::token, 0);
        in >> m_tokenId;
        in >> m_amount;
    }

  public:
    void serialize(QDataStream &out) const override
    {
        Feature::serialize(out);
        out << m_tokenId;
        out << m_amount;
    }
    void addJson(QJsonObject &var) const override
    {
        Feature::addJson(var);
        var.insert("id", m_tokenId.toHexString());
        var.insert("amount", m_amount.toHex());
    }

    [[nodiscard]] auto tokenId(void) const
    {
        return m_tokenId;
    }
    [[nodiscard]] auto amount(void) const
    {
        return m_amount;
    }
    void setTokenId(const TokenID &tokenId)
    {
        m_tokenId = tokenId;
    }
    void setAmount(const uint256 &amount)
    {
        m_amount = amount;
    }
    friend class Feature;
};
} // namespace qiota::qblocks
