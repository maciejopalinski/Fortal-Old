#include "TokenKeyword.h"

const char *keywords[] =
{
    "if",
    "else",
    "for",
    "do",
    "while",
    "return",
    "bool",
    "char",
    "wchar_t",
    "int",
    "float",
    "double",
    "void",
    "signed",
    "unsigned",
    "long",
    "short",
    "const",
    0
};

const char *TokenKeywordType_strings[] =
{
    "TOKEN_KEYWORD_IF",
    "TOKEN_KEYWORD_ELSE",
    "TOKEN_KEYWORD_FOR",
    "TOKEN_KEYWORD_DO",
    "TOKEN_KEYWORD_WHILE",
    "TOKEN_KEYWORD_RETURN",
    "TOKEN_KEYWORD_BOOL",
    "TOKEN_KEYWORD_CHAR",
    "TOKEN_KEYWORD_WCHAR_T",
    "TOKEN_KEYWORD_INT",
    "TOKEN_KEYWORD_FLOAT",
    "TOKEN_KEYWORD_DOUBLE",
    "TOKEN_KEYWORD_VOID",
    "TOKEN_KEYWORD_SIGNED",
    "TOKEN_KEYWORD_UNSIGNED",
    "TOKEN_KEYWORD_LONG",
    "TOKEN_KEYWORD_SHORT",
    "TOKEN_KEYWORD_CONST"
};
const char *TokenKeywordType_enum_to_string(TokenKeywordType index) { return TokenKeywordType_strings[index]; }

TokenKeyword::TokenKeyword(TokenKeywordType type) : type(type) {}

TokenKeyword::~TokenKeyword()
{
    // printf("[DEBUG] TokenKeyword destroyed\n");
}

void TokenKeyword::debug(const char *prefix)
{
    printf
    (
        "%sTokenKeyword(type='%s')\n",
        prefix,
        TokenKeywordType_enum_to_string(type)
    );
}