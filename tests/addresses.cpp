#include "esterv/crypto/block/addresses.hpp"
#include "esterv/crypto/block/features.hpp"
#include "esterv/crypto/block/outputs.hpp"
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

    auto basicOutput = Output::Basic();
    QJsonObject basicOutputJson;
    basicOutput->addJson(basicOutputJson);
    qDebug() << "Basic output:" << basicOutputJson;

    auto nftOutput = Output::NFT(10, 10, {}, {sendFea});
    QJsonObject nftOutputJson;
    nftOutput->addJson(nftOutputJson);
    qDebug() << "NFT output:" << nftOutputJson;

    auto accountOutput = Output::Account();
    QJsonObject accountOutputJson;
    accountOutput->addJson(accountOutputJson);
    qDebug() << "Account output:" << accountOutputJson;

    auto anchorOutput = Output::Anchor();
    QJsonObject anchorOutputJson;
    anchorOutput->addJson(anchorOutputJson);
    qDebug() << "anchor output:" << anchorOutputJson;

    auto foundryOutput = Output::Foundry();
    QJsonObject foundryOutputJson;
    foundryOutput->addJson(foundryOutputJson);
    qDebug() << "Foundry output:" << foundryOutputJson;

    auto delegationOutput = Output::Delegation();
    QJsonObject delegationOutputJson;
    delegationOutput->addJson(delegationOutputJson);
    qDebug() << "Delegation output:" << delegationOutputJson;

    return 0;
}
