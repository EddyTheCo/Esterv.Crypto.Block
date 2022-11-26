#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include <QJsonArray>
namespace qiota{
namespace qblocks{
class c_array : public QByteArray
{
public:
    using QByteArray::QByteArray;
    /*
     *@brief creates a "0x" hex encoded string
     */
    QString toHexString(void)const;
    /*
     *@brief serialize the object to a datastream
     *
     */
    friend QDataStream & operator << (QDataStream &out, const c_array & obj);

    /*
     *@return a data stream object to append data to it
     *
     */
    QDataStream * get_buffer(void);

    c_array(const QJsonValue& val);


};

template<typename max_lenght>
class fl_array : public c_array
{
public:
    using c_array::c_array;
    /*
     *@brief serialize the object to a datastream
     *
     */
   template<typename m_le> friend QDataStream & operator << (QDataStream &out, const fl_array<m_le> & obj);

};


/**
 * @brief The block_id is the BLAKE2b-256 hash of the entire serialized block.
 */
using block_id = c_array;
/**
 * @brief The BLAKE2b-256 hash of the transaction payload containing the referenced output.
 */
using transaction_id = c_array;
using public_key = c_array;
using signature = c_array;

/*
 *
 *
 */
using token_id = c_array;
/**
 * @brief Binary data. A leading uint32 denotes its length.
 */
using dataF = fl_array<quint32>;

/**
 * The tag of the data. A leading uint8 denotes its length.
 **/
using tagF = fl_array<quint8>;

template<typename T>
std::vector<T *> get_T(const QJsonArray& val)
{
    std::vector<T *> var;
    for(const auto& v:val )var.push_back(T::from_Json(v));
    return var;
}
};

};
