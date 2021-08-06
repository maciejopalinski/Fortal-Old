#pragma once
#include "../TokenType/TokenType.h"

enum TokenLiteralType
{
    TOKEN_LITERAL_INT,
    TOKEN_LITERAL_FLOAT,
    TOKEN_LITERAL_CHAR,
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_BOOL,
    TOKEN_LITERAL_NULLPTR
};
extern const char *TokenLiteralType_strings[];
const char *TokenLiteralType_enum_to_string(TokenLiteralType index);

class TokenLiteral
{
    public:
        TokenLiteralType type;

        union
        {
            long long literal_int;
            double literal_float;
            char literal_char;
            char *literal_string;
            bool literal_bool;
            bool literal_nullptr = false;
        };

        TokenLiteral(long long content);
        TokenLiteral(double content);
        TokenLiteral(char content);
        TokenLiteral(char *content, size_t size);
        TokenLiteral(bool content);
        TokenLiteral(void *content);
        ~TokenLiteral();

        void debug(const char *prefix = "");
};