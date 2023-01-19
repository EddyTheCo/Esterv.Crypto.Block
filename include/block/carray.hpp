#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include<QIODevice>
namespace qiota{
namespace qblocks{
class c_array : public QByteArray
{
public:
    using QByteArray::QByteArray;
    c_array(const QByteArray & var):QByteArray(var.constData(),var.size()){};
    /*
     *@brief creates a "0x" hex encoded string
     */
    QString toHexString(void)const;
    /*
     *@brief serialize the object to a datastream
     *
     */
    friend QDataStream & operator << (QDataStream &out, const c_array & obj);
    friend QDataStream & operator >> (QDataStream &in, c_array & obj);
    /*
     *@return a data stream object to append data to it
     *
     */

    template<class obj_type> void from_object(const obj_type& obj)
    {
        auto buffer=QDataStream(this,QIODevice::WriteOnly | QIODevice::Append);
        buffer.setByteOrder(QDataStream::LittleEndian);
        obj.serialize(buffer);
    }
    template<class obj_type> obj_type to_object(void)
    {
        auto buffer=QDataStream(this,QIODevice::ReadOnly);
        buffer.setByteOrder(QDataStream::LittleEndian);
        return obj_type(buffer);
    }
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
    friend QDataStream & operator << (QDataStream &out, const fl_array & obj)
    {

        out<<static_cast<max_lenght>(obj.size());
        out<<static_cast<c_array>(obj);
        return out;

    }
    friend QDataStream & operator >> (QDataStream &in, fl_array & obj)
    {
        max_lenght size;
        in>>size;
        obj.resize(size);
        auto var=static_cast<c_array*>(&obj);
        in>>(*var);

        return in;
    }

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
using NFT_ID =c_array;
using Output_ID =c_array;
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

template<class T>
std::vector<std::shared_ptr<T>> get_T(const QJsonArray& val)
{
    std::vector<std::shared_ptr<T>> var;
    for(const auto& v:val)var.push_back(T::from_(v));
    return var;
}
template<class type_type> type_type get_type(const QJsonValue& val)
{
    return ((type_type)val.toObject()["type"].toInt());
}
template<class type_type> type_type get_type(QDataStream & val)
{
    type_type type_;
    val>>type_;
    return type_;
}

template<class size_type,class obj_type> void serialize_list(QDataStream &out,
                                                             const std::vector<std::shared_ptr<obj_type>> & ptr_vector)
{
    out<<static_cast<size_type>(ptr_vector.size());
    for(const auto& v: ptr_vector)v->serialize(out);
}

template<class size_type,class obj_type> std::vector<std::shared_ptr<obj_type>> deserialize_list(QDataStream &in)
{
    std::vector<std::shared_ptr<obj_type>>  ptr_vector;
    size_type  length_;
    in>>length_;
    for(auto i=0;i<length_;i++)
    {
        ptr_vector.push_back(obj_type::template from_<QDataStream>(in));
    }
    return ptr_vector;
}
template<class obj_type> void order_by_type(std::vector<std::shared_ptr<obj_type>> &ptr_vector)
{
    std::sort(ptr_vector.begin(), ptr_vector.end(), [](auto a, auto b)
    {
        return a->type_m < b->type_m;
    });
}

};

};

