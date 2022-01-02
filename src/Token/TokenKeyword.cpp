#include "TokenKeyword.h"

TokenKeyword::TokenKeyword(TokenKeywordType kind) : TokenHasKind(kind) {}

string TokenKeyword::getPrintableKindString(TokenKeywordType kind)
{
    auto idx = magic_enum::enum_index<TokenKeywordType>(kind);
    if (idx.has_value())
    {
        return lexer_matches[idx.value()];
    }
    return "(null)";
}

string TokenKeyword::getPrintableKindString()
{
    return getPrintableKindString(getKind());
}

TokenKeywordType TokenKeyword::getLexerTypeFromMatch(string match)
{
    auto it = find(lexer_matches.begin(), lexer_matches.end(), match);
    if (it != lexer_matches.end())
    {
        size_t index = it - lexer_matches.begin();
        return (TokenKeywordType) index;
    }
    else return (TokenKeywordType) (-1);
}

const vector<string> TokenKeyword::lexer_matches =
{
    "package",
    "extends",
    "throws",
    "import",
    "static",
    "abstract",
    "final",
    "entrypoint",
    "native",
    "public",
    "protected",
    "private",
    "alias",
    "class",
    "enum",
    "if",
    "else",
    "switch",
    "case",
    "default",
    "for",
    "while",
    "do",
    "break",
    "continue",
    "return",
    "throw",
    "try",
    "catch",
    "finally",
    "new",
    "void",
    "int1",
    "int8",
    "int16",
    "int32",
    "int64",
    "uint8",
    "uint16",
    "uint32",
    "uint64",
    "float",
    "double"
};