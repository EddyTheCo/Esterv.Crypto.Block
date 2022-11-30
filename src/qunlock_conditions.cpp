#include"block/qunlock_conditions.hpp"

namespace qiota{
namespace qblocks{
void Unlock_Condition::serialize(QDataStream &out)const{};
QJsonObject Unlock_Condition::get_Json(void) const{return QJsonObject();};
Unlock_Condition::Unlock_Condition(quint8 typ ):type_m(typ){};
Unlock_Condition * Unlock_Condition::from_Json(const QJsonValue& val){
    const quint8 type_=val.toObject()["type"].toInt();

    switch(type_) {
    case 3:
      return new Expiration_Unlock_Condition(val);
      case 2:
        return new Timelock_Unlock_Condition(val);
      case 1:
        return new Storage_Deposit_Return_Unlock_Condition(val);
      case 0:
        return new Address_Unlock_Condition(val);
        default:
        return nullptr;

    }
}


Address_Unlock_Condition::Address_Unlock_Condition(Address* address_m):Unlock_Condition(0),address_(address_m){};
Address_Unlock_Condition::Address_Unlock_Condition(const QJsonValue& val):
    Address_Unlock_Condition(Address::from_Json(val.toObject()["address"])){};
void Address_Unlock_Condition::serialize(QDataStream &out)const
{
    out<<type_m;
    address_->serialize(out);
}
QJsonObject Address_Unlock_Condition::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("address",address_->get_Json());
    return var;
}


Storage_Deposit_Return_Unlock_Condition::Storage_Deposit_Return_Unlock_Condition(Address* return_address_m, quint64 return_amount_m):Unlock_Condition(1),
    return_address_(return_address_m),
    return_amount_(return_amount_m)
{};
Storage_Deposit_Return_Unlock_Condition::Storage_Deposit_Return_Unlock_Condition(const QJsonValue& val):
    Storage_Deposit_Return_Unlock_Condition(Address::from_Json(val.toObject()["returnAddress"]),
    val.toObject()["returnAmount"].toInt()){};
void Storage_Deposit_Return_Unlock_Condition::serialize(QDataStream &out)const
{
    out<<type_m;
    return_address_->serialize(out);
    out<<return_amount_;
}
QJsonObject Storage_Deposit_Return_Unlock_Condition::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("returnAddress",return_address_->get_Json());
    var.insert("returnAmount",QString::number(return_amount_)); //check this
    return var;
}

Timelock_Unlock_Condition::Timelock_Unlock_Condition(quint32 unix_time_m):Unlock_Condition(2),unix_time_(unix_time_m)
{};
Timelock_Unlock_Condition::Timelock_Unlock_Condition(const QJsonValue& val):
    Timelock_Unlock_Condition(val.toObject()["unixTime"].toInt()){};
void Timelock_Unlock_Condition::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<unix_time_;
}
QJsonObject Timelock_Unlock_Condition::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("unixTime",QString::number(unix_time_));

    return var;
}


Expiration_Unlock_Condition::Expiration_Unlock_Condition(quint32 unix_time_m,Address* return_address_m):Unlock_Condition(3),unix_time_(unix_time_m),
    return_address_(return_address_m)
{};
Expiration_Unlock_Condition::Expiration_Unlock_Condition(const QJsonValue& val):
    Expiration_Unlock_Condition( val.toObject()["unixTime"].toInt(), Address::from_Json(val.toObject()["returnAddress"])){};
void Expiration_Unlock_Condition::serialize(QDataStream &out)const
{
    out<<type_m;
    return_address_->serialize(out);
    out<<unix_time_;
}
QJsonObject Expiration_Unlock_Condition::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",type_m);
    var.insert("returnAddress",return_address_->get_Json());
    var.insert("unixTime",QString::number(unix_time_));
    return var;
}

};
};
