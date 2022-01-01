#pragma once
#include <string.h>

#include "TokenHasKind.h"

enum TokenLiteralType
{
    TOKEN_LITERAL_INT,
    TOKEN_LITERAL_DOUBLE,
    TOKEN_LITERAL_CHAR,
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_BOOL,
    TOKEN_LITERAL_NULL
};

extern const char *TokenLiteralType_printable_strings[];

class TokenLiteral : public TokenHasKind<TokenLiteralType>
{
    private:
        bool is_null = false;
        union
        {
            long long value_int;
            double value_double;
            char *value_string = nullptr;
        };

    public:
        TokenLiteral(TokenLiteralType kind);

        ~TokenLiteral();

        TokenType getType()
        {
            return TOKEN_LITERAL;
        }

        void setValue(long long v);
        void setValue(double v);
        void setValue(const char *v);

        long long getValue(long long);
        double getValue(double);
        char *getValue(string);

        bool isNULL();
        void setNULL(bool null);

        string getValueString();

        static string getPrintableKindString(TokenLiteralType kind);
        string getPrintableKindString();

        string getDebug(string inside = "");
};