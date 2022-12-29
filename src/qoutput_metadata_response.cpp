#include"block/qoutput_metadata_response.hpp"
namespace qiota{
namespace qblocks{
Output_Metadata_Response::Output_Metadata_Response(const QJsonValue& val):
    block_id_(val.toObject()["blockId"]),transaction_id_(val.toObject()["transactionId"]),
    output_index_(val.toObject()["outputIndex"].toInteger()),
    is_spent_(val.toObject()["isSpent"].toBool()){}
};
};
