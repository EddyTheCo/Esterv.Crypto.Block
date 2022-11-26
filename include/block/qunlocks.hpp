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

    Unlock(quint8 typ );
    static Unlock * from_Json(const QJsonValue& val);
    virtual void serialize(QDataStream &out)const;
    virtual QJsonObject get_Json(void) const;

    quint8 type_m;

};

class Signature_Unlock: public Unlock
{
public:
    Signature_Unlock(Signature * signature_m);
    Signature_Unlock(const QJsonValue& val);
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;


private:
 Signature * signature_;

};

class Reference_Unlock: public Unlock
{
public:
    Reference_Unlock(quint16 reference_m);
    Reference_Unlock(const QJsonValue& val);
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
    void serialize(QDataStream &out)const;
    QJsonObject get_Json(void) const;

private:
quint16 nft_reference_unlock_index_;

};
};
};
