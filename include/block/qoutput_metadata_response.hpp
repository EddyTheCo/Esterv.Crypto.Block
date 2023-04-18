#pragma once


#include"block/carray.hpp"
#include<QJsonObject>
namespace qiota{
namespace qblocks{

class  Output_Metadata_Response
{

public:
    Output_Metadata_Response(const QJsonValue& val);
    QJsonObject get_Json()const{
        QJsonObject var;
        var.insert("blockId",block_id_.toHexString());
        var.insert("transactionId",transaction_id_.toHexString());
        var.insert("outputIndex",output_index_);
        var.insert("isSpent",is_spent_);
        var.insert("outputId",outputid_.toHexString());
        return var;
    }
    const Block_ID block_id_;
    const Transaction_ID transaction_id_;
    const quint16 output_index_;
    const bool is_spent_;
    const Output_ID outputid_;
    //const quint32 milestone_index_spent_;
    //const quint32 milestone_timestamp_spent_;
    //const transaction_id transaction_id_spent;

private:
    Output_ID set_outputid(void) const;

};


};
};
