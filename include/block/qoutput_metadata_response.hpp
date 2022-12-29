#pragma once


#include"block/carray.hpp"
#include<QJsonObject>
namespace qiota{
namespace qblocks{

class  Output_Metadata_Response
{

public:
    Output_Metadata_Response(const QJsonValue& val);
    const block_id block_id_;
    const transaction_id transaction_id_;
    const quint16 output_index_;
    const bool is_spent_;
    //const quint32 milestone_index_spent_;
    //const quint32 milestone_timestamp_spent_;
    //const transaction_id transaction_id_spent;
};


};
};
