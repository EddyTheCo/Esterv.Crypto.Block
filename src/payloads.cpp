#include <QCryptographicHash>
#include <esterv/crypto/block/payloads.hpp>

namespace esterv::crypto::block
{

template <class from_type> std::shared_ptr<const Payload> Payload::from(from_type &val)
{

    const auto type = getType<PayloadType>(val);
    switch (type)
    {
    case PayloadType::SignedTransaction:
        return std::shared_ptr<const Payload>(new SignedTransactionPayload(val));
    case PayloadType::TaggedData:
        return std::shared_ptr<const Payload>(new TaggedDataPayload(val));
    default:
        return nullptr;
    }
}

template std::shared_ptr<const Payload> Payload::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<const Payload> Payload::from<QDataStream>(QDataStream &val);
template std::shared_ptr<const Payload> Payload::from<const QJsonValueRef>(const QJsonValueRef &val);

std::shared_ptr<const Payload> Payload::TaggedData(const tagF &tag, const dataF &data)
{
    return std::shared_ptr<const Payload>(new TaggedDataPayload(tag, data));
}

std::shared_ptr<const Payload> Payload::SignedTransaction(const std::shared_ptr<const Essence> &essence_m,
                                                          const pvector<const Unlock> &unlocks_m)
{
    return std::shared_ptr<Payload>(new SignedTransactionPayload(essence_m, unlocks_m));
}

Transaction_Payload::Transaction_Payload(const std::shared_ptr<const Essence> &essence_m,
                                         const pvector<const Unlock> &unlocks_m)
    : essence_(essence_m), unlocks_(unlocks_m), Payload(Transaction_typ){};

Transaction_Payload::Transaction_Payload(const QJsonValue &val)
    : Transaction_Payload(Essence::from_<const QJsonValue>(val.toObject()["essence"]),
                          get_Tvec<Unlock>(val.toObject()["unlocks"].toArray())){};
Transaction_Payload::Transaction_Payload(QDataStream &in)
    : Payload(Transaction_typ), essence_(Essence::from_<QDataStream>(in))
{
    quint16 unlocks_count;
    in >> unlocks_count;
    for (auto i = 0; i < unlocks_count; i++)
    {
        unlocks_.push_back(Unlock::from_<QDataStream>(in));
    }
}

}; // namespace qblocks
