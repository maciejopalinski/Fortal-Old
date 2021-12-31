#pragma once
#include "../Location/Location.h"

enum TokenType
{
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    TOKEN_LITERAL,
    TOKEN_IDENTIFIER,
    TOKEN_COMMENT
};

extern const char *TokenType_printable_strings[];

class Token
{
    public:
        Location location;

        virtual ~Token();

        virtual TokenType getType() = 0;

        const char *getTypeString();
        static const char *getTypeString(TokenType type);

        const char *getPrintableTypeString();
        static const char *getPrintableTypeString(TokenType type);

        virtual string getDebug(string inside = "");

        static string getDescription(TokenType type);
        virtual string getDescription();
};