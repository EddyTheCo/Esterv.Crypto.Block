#include"block/qblock.hpp"
#include<iostream>
#undef NDEBUG
#include <assert.h>
#include<QJsonDocument>

using namespace qiota::qblocks;

int main(int argc, char** argv)
{
    auto data_=dataF("hello world");
    auto tag_=tagF("IOTA");
    auto payload_=std::shared_ptr<Payload>(new Tagged_Data_Payload(tag_,data_));
    c_array serialized_pay;
    serialized_pay.from_object(*payload_);

    qDebug()<<"payload:"<<serialized_pay.toHex();
    auto block_=Block(payload_);
    block_.set_pv(2);

    block_.set_parents({block_id(QByteArray::fromHex("210fc7bb818639ac48a4c6afa2f1581a8b9525e20fda68927f2b2ff836f73578")),
                        block_id(QByteArray::fromHex("db0fa54c29f7fd928d92ca43f193dee47f591549f597a811c8fa67ab031ebd9c"))});
    block_.set_nonce(28110);
   qDebug().noquote()<<"block:\n"<<QString(QJsonDocument(block_.get_Json()).toJson(QJsonDocument::Indented));
    c_array serialized_block;
    serialized_block.from_object(block_);

    qDebug()<<"block:"<<serialized_block.toHex();
    assert(serialized_block.toHex()==
           QByteArray("0202210fc7bb818639ac48a4c6afa2f1581a8b9525e20fda68927f2b2ff836f73578db0fa54c29f7fd928d92ca43f193dee47f591549f597a811c8fa67ab031ebd9c180000000500000004494f54410b00000068656c6c6f20776f726c64ce6d000000000000"));

    auto block2=serialized_block.to_object<Block>();


    c_array serialized_block2;
    serialized_block2.from_object(block2);
    qDebug()<<"block:"<<serialized_block2.toHex();
    assert(serialized_block2.toHex()==
           QByteArray("0202210fc7bb818639ac48a4c6afa2f1581a8b9525e20fda68927f2b2ff836f73578db0fa54c29f7fd928d92ca43f193dee47f591549f597a811c8fa67ab031ebd9c180000000500000004494f54410b00000068656c6c6f20776f726c64ce6d000000000000"));


	return 0;
}
