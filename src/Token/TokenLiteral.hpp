#pragma once
#include "TokenHasType.hpp"
#include <stdlib.h>

enum TokenLiteralType
{
    TOKEN_LITERAL_INT,
    TOKEN_LITERAL_DOUBLE,
    TOKEN_LITERAL_CHAR,
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_BOOL,
    TOKEN_LITERAL_NULL
};

const char *TokenLiteralType_strings[] = {
    "TOKEN_LITERAL_INT",
    "TOKEN_LITERAL_DOUBLE",
    "TOKEN_LITERAL_CHAR",
    "TOKEN_LITERAL_STRING",
    "TOKEN_LITERAL_BOOL",
    "TOKEN_LITERAL_NULL"
};

class TokenLiteral : public TokenHasType<TokenLiteralType>
{
    private:
        bool is_null = false;
        union
        {
            long long value_int;
            double value_double;
            char *value_string;
        };

    public:
        TokenLiteral(TokenLiteralType type) : TokenHasType(type) {}

        ~TokenLiteral()
        {
            if (getType() == TOKEN_LITERAL_STRING) delete[] value_string;
        }

        TokenType getTokenType() { return TOKEN_LITERAL; }
        const char *getTokenTypeString() { return "TokenLiteral"; }
        string getTypeString(TokenLiteralType type) { return TokenLiteralType_strings[type]; }

        void setValue(long long v) { value_int = v; }
        void setValue(double v) { value_double = v; }
        void setValue(const char* v)
        {
            value_string = new char[strlen(v) + 1];
            strcpy(value_string, v);
        }

        long long getValue(long long) { return value_int; }
        double getValue(double) { return value_double; }
        char *getValue(string) { return value_string; }

        bool isNULL() { return is_null; }
        void setNULL(bool null) { is_null = null; }

        string getValueString()
        {
            if (isNULL()) return "null";

            auto type = getType();
            if (type == TOKEN_LITERAL_INT) return format("%lld", getValue((long long) 1));
            if (type == TOKEN_LITERAL_DOUBLE) return format("%f", getValue(0.0f));
            if (type == TOKEN_LITERAL_CHAR) return format("'%c'", getValue((long long) 1));
            if (type == TOKEN_LITERAL_STRING) return format("'%s'", getValue(""));
            if (type == TOKEN_LITERAL_BOOL) return getValue((long long) 1) ? "true" : "false";
            return "<error>";
        }

        string getDebug()
        {
            return format(
                "%s(type='%s', value=%s)",
                getTokenTypeString(),
                getTypeString(getType()).c_str(),
                getValueString().c_str()
            );
        }

        static bool getLexerWhitespaceAfter() { return true; }
        static TokenLiteralType getLexerTypeFromMatch(string match)
        {
            auto it = find(lexer_matches.begin(), lexer_matches.end(), match);
            if (it != lexer_matches.end())
            {
                size_t index = it - lexer_matches.begin();
                return (TokenLiteralType) index;
            }
            else return (TokenLiteralType) -1;
        }
};