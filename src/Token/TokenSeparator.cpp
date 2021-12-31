#include "TokenSeparator.h"

TokenSeparator::TokenSeparator(TokenSeparatorType kind) : TokenHasKind(kind) {}

string TokenSeparator::getPrintableKindString(TokenSeparatorType kind)
{
    string t = " ";
    t[0] = kind;
    return t.c_str();
}

string TokenSeparator::getPrintableKindString()
{
    return getPrintableKindString(getKind());
}

TokenSeparatorType TokenSeparator::getLexerTypeFromMatch(char match)
{
    auto type = magic_enum::enum_cast<TokenSeparatorType>(match);
    return type.value_or((TokenSeparatorType) (-1));
}
