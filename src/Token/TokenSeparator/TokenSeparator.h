#pragma once
#include "../TokenType/TokenType.h"

extern const char separators[];

enum TokenSeparatorType
{
    TOKEN_SEPARATOR_DOT,
    TOKEN_SEPARATOR_COMMA,
    TOKEN_SEPARATOR_SEMICOLON,
    TOKEN_SEPARATOR_COLON,
    TOKEN_SEPARATOR_QUESTION_MARK,
    TOKEN_SEPARATOR_PAREN_L,
    TOKEN_SEPARATOR_PAREN_R,
    TOKEN_SEPARATOR_SQUARE_BRACKET_L,
    TOKEN_SEPARATOR_SQUARE_BRACKET_R,
    TOKEN_SEPARATOR_CURLY_BRACKET_L,
    TOKEN_SEPARATOR_CURLY_BRACKET_R
};
extern const char *TokenSeparatorType_strings[];
const char *TokenSeparatorType_enum_to_string(TokenSeparatorType type);

class TokenSeparator
{
    public:
        TokenSeparatorType type;

        TokenSeparator(TokenSeparatorType type);
        ~TokenSeparator();

        void debug(Location *location);
};