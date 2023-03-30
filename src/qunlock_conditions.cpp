#include"block/qunlock_conditions.hpp"

namespace qiota{
namespace qblocks{
void Unlock_Condition::serialize(QDataStream &out)const{};
QJsonObject Unlock_Condition::get_Json(void) const{return QJsonObject();};
Unlock_Condition::Unlock_Condition(types typ ):type_m(typ){};
template<class from_type>  std::shared_ptr<Unlock_Condition> Unlock_Condition::from_(from_type& val){
    const auto type_=get_type<quint8>(val);

    switch(type_) {
    case types::Expiration_typ:
      return std::shared_ptr<Unlock_Condition>(new Expiration_Unlock_Condition(val));
      case types::Timelock_typ:
        return std::shared_ptr<Unlock_Condition>(new Timelock_Unlock_Condition(val));
      case types::Storage_Deposit_Return_typ:
        return std::shared_ptr<Unlock_Condition>(new Storage_Deposit_Return_Unlock_Condition(val));
      case types::Address_typ:
        return std::shared_ptr<Unlock_Condition>(new Address_Unlock_Condition(val));
      case types::State_Controller_Address_typ:
        return std::shared_ptr<Unlock_Condition>(new State_Controller_Address_Unlock_Condition(val));
    case types::Governor_Address_typ:
      return std::shared_ptr<Unlock_Condition>(new Governor_Address_Unlock_Condition(val));

        default:
        return nullptr;

    }
}

template std::shared_ptr<Unlock_Condition> Unlock_Condition::from_<const QJsonValue>(const QJsonValue& val);
template std::shared_ptr<Unlock_Condition> Unlock_Condition::from_<QDataStream >(QDataStream & val);
template std::shared_ptr<Unlock_Condition> Unlock_Condition::from_<const QJsonValueRef>(const QJsonValueRef& val);
template std::shared_ptr<Unlock_Condition> Unlock_Condition::from_<QJsonValueConstRef const>(QJsonValueConstRef const&);



Address_Unlock_Condition::Address_Unlock_Condition(const std::shared_ptr<Address> &address_m):Unlock_Condition(Address_typ),address_(address_m){};
Address_Unlock_Condition::Address_Unlock_Condition(const QJsonValue& val):
    Address_Unlock_Condition(Address::from_<const QJsonValue>(val.toObject()["address"])){};
Address_Unlock_Condition::Address_Unlock_Condition(QDataStream &in):Unlock_Condition(Address_typ),address_(Address::from_<QDataStream>(in)){};

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

State_Controller_Address_Unlock_Condition::State_Controller_Address_Unlock_Condition(const std::shared_ptr<Address> &address_m):
    Unlock_Condition(State_Controller_Address_typ),address_(address_m){};
State_Controller_Address_Unlock_Condition::State_Controller_Address_Unlock_Condition(const QJsonValue& val):
    State_Controller_Address_Unlock_Condition(Address::from_<const QJsonValue>(val.toObject()["address"])){};
State_Controller_Address_Unlock_Condition::State_Controller_Address_Unlock_Condition(QDataStream &in):
    Unlock_Condition(State_Controller_Address_typ),address_(Address::from_<QDataStream>(in)){};

void State_Controller_Address_Unlock_Condition::serialize(QDataStream &out)const
{
    out<<type_m;
    address_->serialize(out);
}
QJsonObject State_Controller_Address_Unlock_Condition::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("address",address_->get_Json());
    return var;
}
Governor_Address_Unlock_Condition::Governor_Address_Unlock_Condition(const std::shared_ptr<Address> &address_m):
    Unlock_Condition(Governor_Address_typ),address_(address_m){};
Governor_Address_Unlock_Condition::Governor_Address_Unlock_Condition(const QJsonValue& val):
    Governor_Address_Unlock_Condition(Address::from_<const QJsonValue>(val.toObject()["address"])){};
Governor_Address_Unlock_Condition::Governor_Address_Unlock_Condition(QDataStream &in):
    Unlock_Condition(State_Controller_Address_typ),address_(Address::from_<QDataStream>(in)){};

void Governor_Address_Unlock_Condition::serialize(QDataStream &out)const
{
    out<<type_m;
    address_->serialize(out);
}
QJsonObject Governor_Address_Unlock_Condition::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("address",address_->get_Json());
    return var;
}
Storage_Deposit_Return_Unlock_Condition::Storage_Deposit_Return_Unlock_Condition(const std::shared_ptr<Address> &return_address_m, quint64 return_amount_m)
    :Unlock_Condition(Storage_Deposit_Return_typ),
    return_address_(return_address_m),
    return_amount_(return_amount_m)
{};
Storage_Deposit_Return_Unlock_Condition::Storage_Deposit_Return_Unlock_Condition(const QJsonValue& val):
    Storage_Deposit_Return_Unlock_Condition(Address::from_<const QJsonValue>(val.toObject()["returnAddress"]),
    val.toObject()["amount"].toString().toULongLong()){};
Storage_Deposit_Return_Unlock_Condition::Storage_Deposit_Return_Unlock_Condition(QDataStream &in):Unlock_Condition(Storage_Deposit_Return_typ),
return_address_(Address::from_<QDataStream>(in))
{
    in>>return_amount_;
}
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
    var.insert("amount",QString::number(return_amount_)); //check this
    return var;
}

Timelock_Unlock_Condition::Timelock_Unlock_Condition(quint32 unix_time_m):Unlock_Condition(Timelock_typ),unix_time_(unix_time_m)
{};
Timelock_Unlock_Condition::Timelock_Unlock_Condition(const QJsonValue& val):
    Timelock_Unlock_Condition(val.toObject()["unixTime"].toInteger()){};
Timelock_Unlock_Condition::Timelock_Unlock_Condition(QDataStream &in):Unlock_Condition(Timelock_typ)
{
    in>>unix_time_;
}
void Timelock_Unlock_Condition::serialize(QDataStream &out)const
{
    out<<type_m;
    out<<unix_time_;
}
QJsonObject Timelock_Unlock_Condition::get_Json(void) const
{
    QJsonObject var;
    var.insert("type",(int)type_m);
    var.insert("unixTime",(int)unix_time_);

    return var;
}


Expiration_Unlock_Condition::Expiration_Unlock_Condition(quint32 unix_time_m, const std::shared_ptr<Address> &return_address_m):
    Unlock_Condition(Expiration_typ),unix_time_(unix_time_m),
    return_address_(return_address_m)
{};
Expiration_Unlock_Condition::Expiration_Unlock_Condition(const QJsonValue& val):
    Expiration_Unlock_Condition( val.toObject()["unixTime"].toInteger(), Address::from_<const QJsonValue>(val.toObject()["returnAddress"])){};

Expiration_Unlock_Condition::Expiration_Unlock_Condition(QDataStream &in):Unlock_Condition(Expiration_typ),return_address_(Address::from_<QDataStream>(in))
{
    in>>unix_time_;
}
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
    var.insert("unixTime",(int)unix_time_);
    return var;
}

};
};
