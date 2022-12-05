#include<block/carray.hpp>
#include<QIODevice>

namespace qiota{
namespace qblocks{
QString c_array::toHexString(void)const{
    return QString("0x")+QString(toHex());
}
c_array::c_array(const QJsonValue& val):QByteArray(QByteArray::fromHex(val.toString().toLatin1().remove(0,2))){};


QDataStream & operator << (QDataStream &out, const c_array & obj)
{
    out.writeRawData(obj.data(),obj.size());
    return out;
}
QDataStream & operator >> (QDataStream &in, c_array & obj)
{
    in.readRawData(obj.data(),obj.size());
    return in;
}


}
}
