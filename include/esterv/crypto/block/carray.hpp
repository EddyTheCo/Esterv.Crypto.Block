#pragma once

#include "esterv/crypto/bigint.hpp"
#include <QByteArray>
#include <QCryptographicHash>
#include <QDataStream>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <set>

#include <QtCore/QtGlobal>

#if defined(WINDOWS_QBLOCK)
#define QBLOCK_EXPORT Q_DECL_EXPORT
#else
#define QBLOCK_EXPORT Q_DECL_IMPORT
#endif

namespace esterv::crypto::block
{
enum ByteSizes
{
    hash = 32,
    token = 38,
    transactionId = 36
};
enum class UnlockConditionType : quint8
{
    Address = 0,
    StorageDepositReturn = 1,
    Timelock = 2,
    Expiration = 3,
    StateControllerAddress = 4,
    GovernorAddress = 5,
    ImmutableAccountAddress = 6
};
enum class OutputType : quint8
{
    Basic = 0,
    Account = 1,
    Anchor = 2,
    Foundry = 3,
    NFT = 4,
    Delegation = 5,
};
enum class TokenSchemeType : quint8
{
    Simple = 0
};
enum class FeatureType : quint8
{
    Sender = 0,
    Issuer = 1,
    Metadata = 2,
    Tag = 4,
    NativeToken = 5
};
enum class InputType : quint8
{
    UTXO = 0
};
enum class AddressType : quint8
{
    Anchor = 24,
    NFT = 16,
    Account = 8,
    Ed25519 = 0
};
template <class T> class C_Base
{
  protected:
    const T m_type;

    C_Base(T typ) : m_type(typ)
    {
    }
  public:
    virtual void serialize(QDataStream &out) const
    {
        out << m_type;
    }
    virtual void addJson(QJsonObject &var) const
    {
        var.insert("type", (int)m_type);
    }
    auto type(void) const
    {
        return m_type;
    }

    friend inline bool operator<(const C_Base &a, const C_Base &b)
    {
        return a.m_type < b.m_type;
    }
};
/*!
 * \brief Byte Array that takes care of storing the objects in serialized form
 *
 */
class c_array : public QByteArray
{
  public:
    using QByteArray::QByteArray;
    /*!
     * \brief Copy constructor from ByteArray
     */
    c_array(const QByteArray &var) : QByteArray(var.constData(), var.size())
    {
    }
    /*!
     * \brief Constructor from a json value hex string
     */
    c_array(const QJsonValue &val);
    /*!
     *
     * \brief The "0x..." hex encoded string of the data
     * \sa fromHexString()
     */
    QString toHexString(void) const;
    /*!
     * \brief The data from an "0x..." hex encoded string
     * \sa toHexString()
     */
    static c_array fromHexString(QString hash);
    /*!
     * \brief Write to the data stream the obj data
     */
    friend QDataStream &operator<<(QDataStream &out, const c_array &obj);
    /*!
     * \brief Read from datastream to the obj
     */
    friend QDataStream &operator>>(QDataStream &in, c_array &obj);
    /*!
     * \brief Append obj data to the end.
     * For objects that implement QDataStream & operator <<
     */
    template <class obj_type> void append(const obj_type &obj)
    {
        auto buffer = QDataStream(this, QIODevice::WriteOnly | QIODevice::Append);
        buffer.setByteOrder(QDataStream::LittleEndian);
        buffer << obj;
    }
    /*!
     * \brief Get the hash of the data
     */
    template <QCryptographicHash::Algorithm hashty> c_array hash(void) const
    {
        return c_array(QCryptographicHash::hash(*this, hashty));
    }
    /*!
     * \brief Append obj data. Objects that have the serialize(QDataStream) function
     * \sa to_object()
     */
    template <class obj_type> void fromObject(const obj_type &obj)
    {
        auto buffer = QDataStream(this, QIODevice::WriteOnly | QIODevice::Append);
        buffer.setByteOrder(QDataStream::LittleEndian);
        obj->serialize(buffer);
    }
    /*!
     * \brief Return Object from serialized data.
     * Objects that have the from(QDataStream) method.
     * \sa from_object()
     */
    template <class obj_type, std::shared_ptr<const obj_type> (*from)(QDataStream &)>
    std::shared_ptr<const obj_type> toObject(void)
    {
        auto buffer = QDataStream(this, QIODevice::ReadOnly);
        buffer.setByteOrder(QDataStream::LittleEndian);
        return from(buffer);
    }
};

/*!
 * \brief Byte Array with fixed length
 */
template <typename max_lenght> class fl_array : public c_array
{
  public:
    using c_array::c_array;
    /*!
     * \brief Write to the data stream the obj data prepending the obj size as max_lenght type
     */
    friend QDataStream &operator<<(QDataStream &out, const fl_array &obj)
    {
        out << static_cast<max_lenght>(obj.size());
        out << static_cast<c_array>(obj);
        return out;
    }
    /*!
     * \brief Read from datastream to the obj. The size obj the object is read from the datastream
     */
    friend QDataStream &operator>>(QDataStream &in, fl_array &obj)
    {
        max_lenght size;
        in >> size;
        obj.resize(size);
        auto var = static_cast<c_array *>(&obj);
        in >> (*var);

        return in;
    }
};

/*!
 * \brief Is the BLAKE2b-256 hash of the entire serialized block.
 */
using Block_ID = c_array;
/*!
 * \brief Is the BLAKE2b-256 hash of the transaction payload.
 */
using TransactionID = c_array;

using public_key = c_array;
using signature = c_array;

/*!
 * \brief BLAKE2b-256 hash of the Output ID that created the chain of outputs
 */
using ID = c_array;
/*!
 * \brief Concatenation of Transaction_ID || outputIndex
 */
using OutputID = c_array;
/*!
 * \brief The concatenation of Address || Serial Number || Token Scheme Type = Foundry ID
 */
using TokenID = c_array;

/*!
 * \brief Binary data. A leading uint16 denotes its length. Used in
 * https://github.com/iotaledger/tips/blob/main/tips/TIP-0018/tip-0018.md#metadata-feature
 */
using dataf = fl_array<quint16>;

/*!
 * \brief Binary data. A leading uint32 denotes its length. Used in
 * https://github.com/iotaledger/tips/blob/main/tips/TIP-0023/tip-0023.md#serialized-layout
 */
using dataF = fl_array<quint32>;

/*!
 * \brief The tag of the data. A leading uint8 denotes its length. Used in
 * https://github.com/iotaledger/tips/blob/main/tips/TIP-0018/tip-0018.md#tag-feature
 */
using tagF = fl_array<quint8>;

/*!
 * \brief Container of shared pointers.
 */
template <class T> using pvector = std::vector<std::shared_ptr<T>>;

/*!
 * \brief Container of ordered shared pointers.
 */
template <class T> struct ptrLess
{
    bool operator()(const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) const
    {
        return *a < *b;
    }
};

template <class T> using pset = std::set<std::shared_ptr<T>, ptrLess<T>>;

/*!
 *  \return Container of ordered shared pointers from JSON-objects in an Array.
 *  The object has the from_(const QJsonValue& val) function.
 */
template <class T> pset<const T> getT(const QJsonArray &val)
{
    pset<const T> var;
    for (const auto &v : val)
        var.insert(T::from(v));
    return var;
}
/*!
 *  \return Container of shared pointers from JSON-objects in an Array.
 *  The object has the from_(const QJsonValue& val) function.
 */
template <class T> pvector<const T> getTvec(const QJsonArray &val)
{
    pvector<const T> var;
    for (const auto &v : val)
        var.push_back(T::from_(v));
    return var;
}
/*!
 *  \return The type of the object from its JSON form
 */
template <class type_type> type_type getType(const QJsonValue &val)
{
    return ((type_type)val.toObject()["type"].toInt());
}
/*!
 *  \return The type of the object from its serialized form
 */
template <class type_type> type_type getType(QDataStream &val)
{
    type_type type_;
    val >> type_;
    return type_;
}

/*!
 *  \brief append to the datastream the serialized form of objects in a container
 */
template <class size_type, class T> void serializeList(QDataStream &out, const T &container)
{
    out << static_cast<size_type>(container.size());
    for (const auto &v : container)
    {
        v->serialize(out);
    }
}
/*!
 *  \return A container of objects from datastream. The lenght of the container is read from the datastream.
 */
template <class size_type, class obj_type> pset<const obj_type> deserializeList(QDataStream &in)
{
    pset<const obj_type> ptrSet;
    size_type length;
    in >> length;
    for (auto i = 0; i < length; i++)
    {
        ptrSet.insert(obj_type::template from<QDataStream>(in));
    }
    return ptrSet;
}

}; // namespace qblocks
