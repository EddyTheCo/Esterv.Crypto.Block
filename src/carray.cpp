#include <QIODevice>
#include <esterv/crypto/block/carray.hpp>

namespace esterv::crypto::block
{

c_array c_array::fromHexString(QString hash)
{
    return QByteArray::fromHex(hash.toLatin1().remove(0, 2));
}
QString c_array::toHexString(void) const
{
    return QString("0x") + QString(toHex());
}
c_array::c_array(const QJsonValue &val) : QByteArray(QByteArray::fromHex(val.toString().toLatin1().remove(0, 2)))
{
}

QDataStream &operator<<(QDataStream &out, const c_array &obj)
{
    out.writeRawData(obj.data(), obj.size());
    return out;
}
QDataStream &operator>>(QDataStream &in, c_array &obj)
{
    in.readRawData(obj.data(), obj.size());
    return in;
}

} // namespace esterv::crypto::block
