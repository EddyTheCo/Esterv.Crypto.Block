#include "esterv/crypto/block/features.hpp"

namespace esterv::crypto::block
{

Feature::Feature(FeatureType typ) : C_Base<FeatureType>(typ)
{
}
template <class from_type> std::shared_ptr<const Feature> Feature::from(from_type &val)
{
    const auto type = getType<FeatureType>(val);

    switch (type)
    {
    case FeatureType::Metadata:
        return std::shared_ptr<Feature>(new MetadataFeature(val));
    case FeatureType::Tag:
        return std::shared_ptr<Feature>(new TagFeature(val));
    case FeatureType::Sender:
        return std::shared_ptr<Feature>(new SenderFeature(val));
    case FeatureType::Issuer:
        return std::shared_ptr<Feature>(new IssuerFeature(val));
    case FeatureType::NativeToken:
        return std::shared_ptr<Feature>(new NativeTokenFeature(val));
    default:
        return nullptr;
    }
}
template std::shared_ptr<const Feature> Feature::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const Feature> Feature::from<QDataStream>(QDataStream &val);

std::shared_ptr<const Feature> Feature::Sender(const std::shared_ptr<const Address> &sender)
{
    return std::shared_ptr<const Feature>(new SenderFeature(sender));
}

std::shared_ptr<const Feature> Feature::Issuer(const std::shared_ptr<const Address> &issuer)
{
    return std::shared_ptr<Feature>(new IssuerFeature(issuer));
}

MetadataEntries::MetadataEntries(const QJsonValue &val)
{
}
MetadataEntries::MetadataEntries(QDataStream &in)
{
    quint8 entriesCount;
    in >> entriesCount;
    for (auto i = 0; i < entriesCount; ++i)
    {
        fl_array<quint8> key;
        in >> key;
        fl_array<quint16> value;
        in >> value;
        insert({key, value});
    }
}
QJsonArray MetadataEntries::getJson() const
{
    QJsonArray var;
    for (const auto &v : *this)
    {
        var.push_back(QJsonObject{{v.first, v.second.toHexString()}});
    }
    return var;
}
std::shared_ptr<const Feature> Feature::Metadata(const MetadataEntries &entries)
{
    return std::shared_ptr<const Feature>(new MetadataFeature(entries));
}

std::shared_ptr<const Feature> Feature::Tag(const tagF &tag)
{
    return std::shared_ptr<const Feature>(new TagFeature(tag));
}

} // namespace qiota::qblocks
