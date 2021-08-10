#pragma once
#include "../TokenType/TokenType.h"

extern const char *keywords[];

enum TokenKeywordType
{
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_DO,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_BOOL,
    TOKEN_KEYWORD_CHAR,
    TOKEN_KEYWORD_WCHAR_T,
    TOKEN_KEYWORD_INT,
    TOKEN_KEYWORD_FLOAT,
    TOKEN_KEYWORD_DOUBLE,
    TOKEN_KEYWORD_VOID,
    TOKEN_KEYWORD_SIGNED,
    TOKEN_KEYWORD_UNSIGNED,
    TOKEN_KEYWORD_LONG,
    TOKEN_KEYWORD_SHORT,
    TOKEN_KEYWORD_CONST
};
extern const char *TokenKeywordType_strings[];
const char *TokenKeywordType_enum_to_string(TokenKeywordType index);

class TokenKeyword
{
    public:
        TokenKeywordType type;

        TokenKeyword(TokenKeywordType type);
        ~TokenKeyword();

        void debug(Location *location);
};