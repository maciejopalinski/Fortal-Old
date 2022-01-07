#include "Token.h"
#include "MagicEnum.hpp"

const char *TokenType_printable_strings[] =
{
    "keyword",
    "operator",
    "separator",
    "literal",
    "identifier",
    "comment"
};

Token::~Token() {}

const char *Token::getTypeString()
{
    return getTypeString(getType());
}

const char *Token::getTypeString(TokenType type)
{
    auto name = magic_enum::enum_name<TokenType>(type);
    return name.data();
}

const char *Token::getPrintableTypeString()
{
    return getPrintableTypeString(getType());
}

const char *Token::getPrintableTypeString(TokenType type)
{
    auto idx = magic_enum::enum_index<TokenType>(type);
    if (idx.has_value())
    {
        return TokenType_printable_strings[idx.value()];
    }
    return "(null)";
}

string Token::getDebug(string inside)
{
    return
        format(
            "%s(%s)",
            getTypeString(),
            inside.c_str()
        );
}

string Token::getDescription(TokenType type)
{
    return getPrintableTypeString(type);
}

string Token::getDescription()
{
    return getDescription(getType());
}