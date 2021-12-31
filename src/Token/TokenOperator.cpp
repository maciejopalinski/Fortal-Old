#include "TokenOperator.h"

TokenOperator::TokenOperator(TokenOperatorType kind) : TokenHasKind(kind) {}

string TokenOperator::getPrintableKindString(TokenOperatorType kind)
{
    auto idx = magic_enum::enum_index<TokenOperatorType>(kind);
    if (idx.has_value())
    {
        return lexer_matches[idx.value()];
    }
    return "(null)";
}

string TokenOperator::getPrintableKindString()
{
    return getPrintableKindString(getKind());
}

TokenOperatorType TokenOperator::getLexerTypeFromMatch(string match)
{
    auto it = find(lexer_matches.begin(), lexer_matches.end(), match);
    if (it != lexer_matches.end())
    {
        size_t index = it - lexer_matches.begin();
        return (TokenOperatorType) index;
    }
    else return (TokenOperatorType) (-1);
}

const vector<string> TokenOperator::lexer_matches =
{
    "+", "-", "*", "/", "%",
    "~", "|", "&", "^", "<<", ">>",
    "++", "--",
    "=", "+=", "-=", "*=", "/=", "%=",
    "~=", "|=", "&=", "^=", "<<=", ">>=",
    "==", "!=", ">", "<", ">=", "<=",
    "!", "||", "&&", "?",
    "@", "#", "$"
};