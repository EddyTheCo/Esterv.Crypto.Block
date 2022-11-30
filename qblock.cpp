#include"block/qblock.hpp"

namespace qiota{
	namespace qblocks{

		Block::Block(Payload* pyl):
			payload_(pyl){};
		void Block::set_pv(const quint8& pv)
		{
			protocol_version=pv;
		}
		void Block::set_parents(const std::vector<block_id>& parents_m){
			parents_ = std::move(parents_m);
		}
		void Block::set_nonce(const quint64& nonce_m){
			nonce_=nonce_m;
		}
		QJsonObject Block::get_Json(void)const
		{
			QJsonObject var;
			var.insert("protocolVersion",(int)protocol_version);
			QJsonArray parentarr;
			for(const auto& v: parents_)parentarr.push_back(v.toHexString());
			var.insert("parents",parentarr);
			if(payload_)
			{
				var.insert("payload",payload_->get_Json());
				
			}
			else
			{
				var.insert("payload",QJsonValue::Null);
			}
			var.insert("nonce",QString::number(nonce_));
			return var;
		}

		QDataStream & operator < (QDataStream &out, const Block & obj)
		{
			out<<obj.protocol_version<<static_cast<quint8>(obj.parents_.size());

			for(const auto& v:obj.parents_)out<<v;

			if(obj.payload_)
			{
				c_array serialized_payload;
				obj.payload_->serialize(*serialized_payload.get_buffer());
				out<<static_cast<quint32>(serialized_payload.size());
				out<<serialized_payload;
			}
			else
			{
				out<<static_cast<quint32>(0);
			}
			return out;
		}

		QDataStream & operator << (QDataStream &out, const Block & obj)
		{
			out<obj;
			out<<obj.nonce_;
			return out;
		}

	}


}
