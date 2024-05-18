#pragma once

#include "qbigint.hpp"
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

namespace qiota
{
namespace qblocks
{
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
    c_array(const QByteArray &var) : QByteArray(var.constData(), var.size()){};
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
    template <class obj_type> void from_object(const obj_type &obj)
    {
        auto buffer = QDataStream(this, QIODevice::WriteOnly | QIODevice::Append);
        buffer.setByteOrder(QDataStream::LittleEndian);
        obj.serialize(buffer);
    }
    /*!
     * \brief Return Object from serialized data.
     * Objects that implement the constructor(QDataStream &in)
     * \sa from_object()
     */
    template <class obj_type> obj_type to_object(void)
    {
        auto buffer = QDataStream(this, QIODevice::ReadOnly);
        buffer.setByteOrder(QDataStream::LittleEndian);
        return obj_type(buffer);
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
using Transaction_ID = c_array;

using public_key = c_array;
using signature = c_array;

/*!
 * \brief BLAKE2b-256 hash of the Output ID that created the NFT chain of outputs
 */
using NFT_ID = c_array;
/*!
 * \brief BLAKE2b-256 hash of the Output ID that created the Alias chain of outputs
 */
using Alias_ID = c_array;
/*!
 * \brief Concatenation of Transaction_ID || outputIndex
 */
using Output_ID = c_array;
/*!
 * \brief The concatenation of Address || Serial Number || Token Scheme Type = Foundry ID
 */
using Token_ID = c_array;

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
template <class T> pset<const T> get_T(const QJsonArray &val)
{
    pset<const T> var;
    for (const auto &v : val)
        var.insert(T::from_(v));
    return var;
}
/*!
 *  \return Container of shared pointers from JSON-objects in an Array.
 *  The object has the from_(const QJsonValue& val) function.
 */
template <class T> pvector<const T> get_Tvec(const QJsonArray &val)
{
    pvector<const T> var;
    for (const auto &v : val)
        var.push_back(T::from_(v));
    return var;
}
/*!
 *  \return The type of the object from its JSON form
 */
template <class type_type> type_type get_type(const QJsonValue &val)
{
    return ((type_type)val.toObject()["type"].toInt());
}
/*!
 *  \return The type of the object from its serialized form
 */
template <class type_type> type_type get_type(QDataStream &val)
{
    type_type type_;
    val >> type_;
    return type_;
}

/*!
 *  \brief append to the datastream the serialized form of objects in a container
 */
template <class size_type, class obj_type> void serializeList(QDataStream &out, const pset<const obj_type> &ptrSet)
{
    out << static_cast<size_type>(ptrSet.size());
    for (const auto &v : ptrSet)
        v->serialize(out);
}
/*!
 *  \return A container of objects from datastream. The lenght of the container is read from the datastream.
 */
template <class size_type, class obj_type> pset<const obj_type> deserializeList(QDataStream &in)
{
    pset<const obj_type> ptrSet;
    size_type length_;
    in >> length_;
    for (auto i = 0; i < length_; i++)
    {
        ptrSet.insert(obj_type::template from_<QDataStream>(in));
    }
    return ptrSet;
}
/*!
 *  \brief Order a Container of shared pointer to objects
 *  Normally ordered by type.
 */
template <class obj_type> void orderList(pvector<obj_type> &ptrVector)
{
    std::sort(ptrVector.begin(), ptrVector.end(), [](auto a, auto b) { return *a < *b; });
}

}; // namespace qblocks

}; // namespace qiota
