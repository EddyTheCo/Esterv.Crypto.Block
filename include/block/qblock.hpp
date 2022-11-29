#pragma once
/**
*
*  https://github.com/iotaledger/tips/blob/main/tips/TIP-0024/tip-0024.md
*
**/

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include<QJsonArray>
#include "block/carray.hpp"
#include "block/qsignatures.hpp"
#include "block/qsignatures.hpp"
#include "block/qpayloads.hpp"
#include "block/qunlocks.hpp"
#include "block/qessences.hpp"
#include "block/qinputs.hpp"
#include "block/qoutputs.hpp"
#include "block/qnative_tokens.hpp"
#include "block/qunlock_conditions.hpp"
#include "block/qfeatures.hpp"
#include "block/qaddresses.hpp"

namespace qiota{

namespace qblocks{
class Block
{
	public: 

    Block(Payload* pyl);

    friend QDataStream & operator < (QDataStream &out, const Block & obj);


    friend QDataStream & operator << (QDataStream &out, const Block & obj);


    QJsonObject get_Json(void)const;

    void set_parents(const std::vector<block_id>& parents_m);

    void set_nonce(const quint64& nonce_m);
    void set_pv(const quint8& pv);

    private:

    quint8 protocol_version;
    quint64 nonce_;
    Payload* payload_;
    std::vector<block_id> parents_;

};

};
};
