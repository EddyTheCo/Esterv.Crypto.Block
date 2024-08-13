#include "esterv/crypto/block/token_scheme.hpp"

namespace esterv::crypto::block
{

template <class from_type> std::shared_ptr<TokenScheme> TokenScheme::from(from_type &val)
{
    const auto type = getType<TokenSchemeType>(val);

    switch (type)
    {
    case TokenSchemeType::Simple:
        return std::shared_ptr<TokenScheme>(new SimpleTokenScheme(val));
    default:
        return nullptr;
    }
}
template std::shared_ptr<TokenScheme> TokenScheme::from<const QJsonValue>(const QJsonValue &val);
template std::shared_ptr<TokenScheme> TokenScheme::from<QDataStream>(QDataStream &val);

std::shared_ptr<TokenScheme> TokenScheme::Simple(const uint256 &mintedTokens, const uint256 &meltedTokens,
                                                 const uint256 &maximumSupply)
{
    return std::shared_ptr<TokenScheme>(new SimpleTokenScheme(mintedTokens, meltedTokens, maximumSupply));
}

}; // namespace qblocks
