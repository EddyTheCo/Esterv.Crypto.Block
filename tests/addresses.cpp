#include "esterv/crypto/block/addresses.hpp"
#include "esterv/crypto/block/features.hpp"
#include "esterv/crypto/block/outputs.hpp"
#include "esterv/crypto/block/unlock_conditions.hpp"
#undef NDEBUG
#include <assert.h>

using namespace esterv::crypto::block;

int main(int argc, char **argv)
{
    const auto nftAddr =
        Address::NFT(c_array::fromHexString("0x56fdfc072182654f163f5f0f9a621d729566c74d10037c4d7bbb0407d1e2c649"));
    c_array binSer;
    binSer.fromObject(nftAddr);
    qDebug() << binSer.toHexString();

    const auto Addr1 =
        Address::Ed25519(c_array::fromHexString("0x52fdfc072182654f163f5f0f9a621d729566c74d10037c4d7bbb0407d1e2c649"));

    const auto Addr2 =
        Address::Ed25519(c_array::fromHexString("0x53fdfc072182654f163f5f0f9a621d729566c74d10037c4d7bbb0407d1e2c649"));

    const auto Addr3 =
        Address::Ed25519(c_array::fromHexString("0x54fdfc072182654f163f5f0f9a621d729566c74d10037c4d7bbb0407d1e2c649"));

    const auto Addr4 =
        Address::Account(c_array::fromHexString("0x55fdfc072182654f163f5f0f9a621d729566c74d10037c4d7bbb0407d1e2c649"));

    const auto Addr5 =
        Address::NFT(c_array::fromHexString("0x56fdfc072182654f163f5f0f9a621d729566c74d10037c4d7bbb0407d1e2c649"));

    const auto multiAddr = Address::Multi({{Addr5, 3}, {Addr4, 2}, {Addr3, 1}, {Addr2, 1}, {Addr1, 1}}, 5);

    QJsonObject multiAddrJson;
    multiAddr->addJson(multiAddrJson);
    qDebug() << "multiAddress:" << multiAddrJson;

    auto var = binSer.toObject<Address, Address::from>();

    qDebug() << var->addrHash().toHexString();

    QJsonObject nftAddrJson;
    nftAddr->addJson(nftAddrJson);
    qDebug() << nftAddrJson;

    const auto addressUC = UnlockCondition::Address(nftAddr);

    const auto sendFea = Feature::Sender(nftAddr);
    QJsonObject sendFeaJson;
    sendFea->addJson(sendFeaJson);
    qDebug() << sendFeaJson;

    auto basicOutput = Output::Basic(0, 0, {addressUC});
    QJsonObject basicOutputJson;
    basicOutput->addJson(basicOutputJson);
    qDebug() << "Basic output:" << basicOutputJson;

    auto nftOutput = Output::NFT(10, 10, {addressUC}, {sendFea});
    QJsonObject nftOutputJson;
    nftOutput->addJson(nftOutputJson);
    qDebug() << "NFT output:" << nftOutputJson;

    auto accountOutput = Output::Account(100, 100, 1, {addressUC});
    QJsonObject accountOutputJson;
    accountOutput->addJson(accountOutputJson);
    qDebug() << "Account output:" << accountOutputJson;

    auto anchorOutput = Output::Anchor(50, 50, 1, {addressUC});
    QJsonObject anchorOutputJson;
    anchorOutput->addJson(anchorOutputJson);
    qDebug() << "anchor output:" << anchorOutputJson;

    const auto tokenScheme = TokenScheme::Simple();
    auto foundryOutput = Output::Foundry(30, 30, tokenScheme, {addressUC});
    QJsonObject foundryOutputJson;
    foundryOutput->addJson(foundryOutputJson);
    qDebug() << "Foundry output:" << foundryOutputJson;

    auto delegationOutput = Output::Delegation(30, 10, nftAddr, 0, 0, {addressUC});
    QJsonObject delegationOutputJson;
    delegationOutput->addJson(delegationOutputJson);
    qDebug() << "Delegation output:" << delegationOutputJson;

    return 0;
}
