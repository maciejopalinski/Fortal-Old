#include "TokenType.h"

char *TokenType_strings[] =
{
    "TOKEN_NULL",
    "TOKEN_COMMENT",
    "TOKEN_KEYWORD",
    "TOKEN_SEPARATOR",
    "TOKEN_OPERATOR",
    "TOKEN_LITERAL",
    "TOKEN_IDENTIFIER"
};

const char *TokenType_enum_to_string(TokenType index) { return TokenType_strings[index]; }