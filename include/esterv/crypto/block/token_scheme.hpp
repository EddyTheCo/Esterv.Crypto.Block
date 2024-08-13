#pragma once

#include "esterv/crypto/block/carray.hpp"

namespace esterv::crypto::block
{

class TokenScheme : public C_Base<TokenSchemeType>
{
  protected:
    TokenScheme(TokenSchemeType typ) : C_Base<TokenSchemeType>(typ)
    {
    }

  public:
    template <class from_type> static std::shared_ptr<TokenScheme> from(from_type &val);

    [[nodiscard]] static std::shared_ptr<TokenScheme> Simple(const uint256 &mintedTokens, const uint256 &meltedTokens,
                                                             const uint256 &maximumSupply);
};

class SimpleTokenScheme : public TokenScheme
{
    uint256 m_mintedTokens, m_meltedTokens, m_maximumSupply;
    SimpleTokenScheme(const uint256 &mintedTokens, const uint256 &meltedTokens, const uint256 &maximumSupply)
        : TokenScheme{TokenSchemeType::Simple}, m_mintedTokens{mintedTokens}, m_meltedTokens{meltedTokens},
          m_maximumSupply{maximumSupply}
    {
    }
    SimpleTokenScheme(const QJsonValue &val)
        : TokenScheme{TokenSchemeType::Simple}, m_mintedTokens{val.toObject()["mintedTokens"].toString()},
          m_meltedTokens{val.toObject()["meltedTokens"].toString()},
          m_maximumSupply{val.toObject()["maximumSupply"].toString()}
    {
    }
    SimpleTokenScheme(QDataStream &in) : TokenScheme{TokenSchemeType::Simple}
    {
        in >> m_mintedTokens;
        in >> m_meltedTokens;
        in >> m_maximumSupply;
    }

  public:
    void serialize(QDataStream &out) const override
    {
        C_Base::serialize(out);
        out << m_mintedTokens;
        out << m_meltedTokens;
        out << m_maximumSupply;
    }
    void addJson(QJsonObject &var) const override
    {
        TokenScheme::addJson(var);
        var.insert("mintedTokens", m_mintedTokens.toHex());
        var.insert("meltedTokens", m_meltedTokens.toHex());
        var.insert("maximumSupply", m_maximumSupply.toHex());
    }
    [[nodiscard]] auto mintedTokens(void) const
    {
        return m_mintedTokens;
    }
    [[nodiscard]] auto meltedTokens(void) const
    {
        return m_meltedTokens;
    }
    [[nodiscard]] auto maximumSupply(void) const
    {
        return m_maximumSupply;
    }

    void setMintedTokens(const uint256 &mintedTokens)
    {
        m_mintedTokens = mintedTokens;
    }
    void setMeltedTokens(const uint256 &meltedTokens)
    {
        m_meltedTokens = meltedTokens;
    }
    void setmaximumSupply(const uint256 &maximumSupply)
    {
        m_maximumSupply = maximumSupply;
    }

    friend class TokenScheme;
};

}; // namespace qiota
