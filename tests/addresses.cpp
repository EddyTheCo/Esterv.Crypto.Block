#include "esterv/crypto/block/addresses.hpp"
#include "esterv/crypto/block/features.hpp"
#undef NDEBUG
#include <assert.h>

using namespace esterv::crypto::block;

int main(int argc, char **argv)
{
    const auto nftAddr =
        Address::NFT(c_array::fromHexString("0xd45b3583d5f4075872753aee16fdf427a71cc86ea6debf867eef78c89f2ba88e"));
    c_array binSer;
    binSer.fromObject(nftAddr);
    qDebug() << binSer.toHexString();

    auto var = binSer.toObject<Address, Address::from>();

    qDebug() << var->addrHash().toHexString();

    QJsonObject nftAddrJson;
    nftAddr->addJson(nftAddrJson);
    qDebug() << nftAddrJson;

    const auto sendFea = Feature::Sender(nftAddr);
    QJsonObject sendFeaJson;
    sendFea->addJson(sendFeaJson);
    qDebug() << sendFeaJson;
    return 0;
}
