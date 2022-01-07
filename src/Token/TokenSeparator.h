#pragma once
#include "TokenHasKind.h"

enum TokenSeparatorType
{
    TOKEN_SEPARATOR_DOT = '.',
    TOKEN_SEPARATOR_COMMA = ',',
    TOKEN_SEPARATOR_SEMICOLON = ';',
    TOKEN_SEPARATOR_COLON = ':',
    TOKEN_SEPARATOR_BRACKET_PAREN_L = '(',
    TOKEN_SEPARATOR_BRACKET_PAREN_R = ')',
    TOKEN_SEPARATOR_BRACKET_SQUARE_L = '[',
    TOKEN_SEPARATOR_BRACKET_SQUARE_R = ']',
    TOKEN_SEPARATOR_BRACKET_CURLY_L = '{',
    TOKEN_SEPARATOR_BRACKET_CURLY_R = '}'
};

class TokenSeparator : public TokenHasKind<TokenSeparatorType>
{
    public:
        static const bool lexer_whitespace_after = false;

        TokenSeparator(TokenSeparatorType kind);

        TokenType getType() override
        {
            return TOKEN_SEPARATOR;
        }

        static string getPrintableKindString(TokenSeparatorType kind);
        string getPrintableKindString() override;

        static TokenSeparatorType getLexerTypeFromMatch(char match);
};