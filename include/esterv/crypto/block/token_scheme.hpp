#pragma once

#include "block/carray.hpp"
#include <QByteArray>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonValue>
namespace qiota ::qblocks
{

class TokenScheme
{
  public:
    enum types : quint8
    {
        Simple_typ = 0
    };
    TokenScheme(types typ);
    template <class from_type> static std::shared_ptr<TokenScheme> from_(from_type &val);

    template <class... Args> static auto Simple(Args &&...args);
    virtual void serialize(QDataStream &out) const;
    virtual QJsonObject getJson(void) const;

    types type(void) const
    {
        return m_type;
    }

  private:
    const types m_type;
};

class SimpleTokenScheme : public TokenScheme
{
  public:
    SimpleTokenScheme(const quint256 &minted_tokens_m, const quint256 &melted_tokens_m,
                      const quint256 &maximum_supply_m);
    SimpleTokenScheme(const QJsonValue &val);
    SimpleTokenScheme(QDataStream &in);
    void serialize(QDataStream &out) const;

    QJsonObject get_Json(void) const;

  private:
    quint256 minted_tokens_, melted_tokens_, maximum_supply_;
};

}; // namespace qiota
