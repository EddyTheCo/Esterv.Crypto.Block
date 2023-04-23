#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include"block/carray.hpp"
#include"block/qsignatures.hpp"
namespace qiota{
namespace qblocks{
class  Unlock
{
public:
    enum types : quint8 { Signature_typ, Reference_typ, Alias_typ, NFT_typ };


    Unlock(types typ );
    template<typename from_type> static std::shared_ptr<const Unlock> from_(from_type& val);
    template<class deriv, class... Types> static std::shared_ptr<const Unlock> get(Types... args)
    {
        return std::shared_ptr<const Unlock>(new deriv(args...));
    }

    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    types type(void)const{return type_m;}

private:
    const types type_m;

};

class Signature_Unlock: public Unlock
{
public:
    Signature_Unlock(const std::shared_ptr<const Signature>& signature_m);
    Signature_Unlock(const QJsonValue& val);
    Signature_Unlock(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;

private:
    std::shared_ptr<const Signature> signature_;

};

class Reference_Unlock: public Unlock
{
public:
    Reference_Unlock(quint16 reference_m);
    Reference_Unlock(const QJsonValue& val);
    Reference_Unlock(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;


private:
    quint16 reference_;

};

class Alias_Unlock: public Unlock
{
public:
    Alias_Unlock(quint16 alias_reference_unlock_index_m);
    Alias_Unlock(const QJsonValue& val);
    Alias_Unlock(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;


private:
    quint16 alias_reference_unlock_index_;

};

class NFT_Unlock: public Unlock
{
public:
    NFT_Unlock(quint16 nft_reference_unlock_index_m);
    NFT_Unlock(const QJsonValue& val);
    NFT_Unlock(QDataStream &in);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;

private:
    quint16 nft_reference_unlock_index_;

};
};
};
