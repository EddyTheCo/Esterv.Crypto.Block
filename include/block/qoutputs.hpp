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
            enum types : quint8 { Basic_typ=3 };
                Output(types typ );
				template<class from_type> static std::shared_ptr<Output> from_(from_type& val);
                template<class derived_> std::shared_ptr<derived_> to(void)const;
				virtual void serialize(QDataStream &out)const;
				virtual QJsonObject get_Json(void) const;
                const types type_m;
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
                quint64 min_deposit_of_output(const quint64 &wkey, const quint64 &wdata, const quint64 &v_byte_cost)const;
				QJsonObject get_Json(void) const;
				quint64 amount(void)const{return amount_;};
                void set_amount(quint64 amount_m){amount_=amount_m;}
                std::shared_ptr<Unlock_Condition> get_unlock_(const Unlock_Condition::types& typ)const
                {
                    const auto found=std::find_if(unlock_conditions_.begin(),unlock_conditions_.end(),
                                                  [typ](const auto& it){return (it->type_m==typ);});
                    return (found==unlock_conditions_.end())?nullptr:*found;
                }

                std::shared_ptr<Feature> get_feature_(const Feature::types& typ)const
                {
                    const auto found=std::find_if(features_.begin(),features_.end(),
                                                  [typ](const auto& it){return (it->type_m==typ);});
                    return (found==features_.end())?nullptr:*found;
                }
				std::vector<std::shared_ptr<Feature>> features(void)const{return features_;};
				std::vector<std::shared_ptr<Unlock_Condition>> unlock_conditions(void)const{return unlock_conditions_;};
					

			private:

				std::vector<std::shared_ptr<Unlock_Condition>> unlock_conditions_;
				std::vector<std::shared_ptr<Feature>> features_;
				std::vector<std::shared_ptr<Native_Token>> native_tokens_;
				quint64 amount_;
		};

	};

};
