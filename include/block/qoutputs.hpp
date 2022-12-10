#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QJsonValue>
#include<QJsonObject>
#include<QJsonArray>
#include"block/carray.hpp"
#include"block/qunlock_conditions.hpp"
#include"block/qfeatures.hpp"
#include"block/qnative_tokens.hpp"
namespace qiota{
	namespace qblocks{

		class Output
		{
			public:
				Output(quint8 typ );
				template<class from_type> static std::shared_ptr<Output> from_(from_type& val);
				virtual void serialize(QDataStream &out)const;
				virtual QJsonObject get_Json(void) const;
				quint8 type_m;
		};

		class Basic_Output :public Output
		{
			public:
				Basic_Output(quint64 amount_m, const std::vector<std::shared_ptr<Unlock_Condition>> & unlock_conditions_m,
						const std::vector<std::shared_ptr<Feature>>& features_m,
						const std::vector<std::shared_ptr<Native_Token>>& native_tokens_m);
				Basic_Output(const QJsonValue& val);
				Basic_Output(QDataStream &in);
				void serialize(QDataStream &out)const;

				QJsonObject get_Json(void) const;
				quint64 amount(void)const{return amount_;};
				std::vector<std::shared_ptr<Feature>> features(void){return features_;};
				std::vector<std::shared_ptr<Unlock_Condition>> unlock_conditions(void){return unlock_conditions_;};
					

			private:

				std::vector<std::shared_ptr<Unlock_Condition>> unlock_conditions_;
				std::vector<std::shared_ptr<Feature>> features_;
				std::vector<std::shared_ptr<Native_Token>> native_tokens_;
				quint64 amount_;
		};

	};

};
