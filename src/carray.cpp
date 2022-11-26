#include<block/carray.hpp>
#include<QIODevice>

namespace qiota{
namespace qblocks{
QString c_array::toHexString(void)const{
    return QString("0x")+QString(toHex());
}
c_array::c_array(const QJsonValue& val):QByteArray(QByteArray::fromHex(val.toString().toLatin1().remove(0,2))){};

QDataStream * c_array::get_buffer(void){
    auto buffer=new QDataStream(this,QIODevice::WriteOnly | QIODevice::Append);
    buffer->setByteOrder(QDataStream::LittleEndian);
    return buffer;
}
QDataStream & operator << (QDataStream &out, const c_array & obj)
{
    out.writeRawData(obj.data(),obj.size());
    return out;
}
template<typename max_lenght>
QDataStream & operator << (QDataStream &out, const fl_array<max_lenght> & obj)
{
    out<<static_cast<max_lenght>(obj.size());
    out<<static_cast<c_array>(obj);
    return out;
}

template QDataStream & operator << <quint32> (QDataStream &out, const fl_array<quint32> & obj);
template QDataStream & operator << <quint8> (QDataStream &out, const fl_array<quint8> & obj);
}
}
