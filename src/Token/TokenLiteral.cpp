#include "TokenLiteral.h"

const char *TokenLiteralType_printable_strings[] =
{
    "int",
    "double",
    "char",
    "string",
    "bool",
    "null"
};

TokenLiteral::TokenLiteral(TokenLiteralType kind) : TokenHasKind(kind) {}

TokenLiteral::~TokenLiteral()
{
    if (getKind() == TOKEN_LITERAL_STRING && value_string) delete[] value_string;
}

void TokenLiteral::setValue(long long v)
{
    value_int = v;
}

void TokenLiteral::setValue(double v)
{
    value_double = v;
}

void TokenLiteral::setValue(const char *v)
{
    value_string = new char[strlen(v) + 1];
    strcpy(value_string, v);
}

long long TokenLiteral::getValue(long long)
{
    return value_int;
}

double TokenLiteral::getValue(double)
{
    return value_double;
}

char *TokenLiteral::getValue(string)
{
    return value_string;
}

bool TokenLiteral::isNULL()
{
    return is_null;
}

void TokenLiteral::setNULL(bool null)
{
    is_null = null;
}

string TokenLiteral::getValueString()
{
    if (isNULL()) return "null";

    auto kind = getKind();
    if (kind == TOKEN_LITERAL_INT) return format("%lld", getValue((long long) 1));
    if (kind == TOKEN_LITERAL_DOUBLE) return format("%f", getValue(0.0f));
    if (kind == TOKEN_LITERAL_CHAR) return format("'%c'", getValue((long long) 1));
    if (kind == TOKEN_LITERAL_STRING) return format("'%s'", getValue(""));
    if (kind == TOKEN_LITERAL_BOOL) return getValue((long long) 1) ? "true" : "false";
    return "<error>";
}

string TokenLiteral::getPrintableKindString(TokenLiteralType kind)
{
    auto idx = magic_enum::enum_index<TokenLiteralType>(kind);
    if (idx.has_value())
    {
        return TokenLiteralType_printable_strings[idx.value()];
    }
    return "(null)";
}

string TokenLiteral::getPrintableKindString()
{
    return getPrintableKindString(getKind());
}

string TokenLiteral::getDebug(const string &inside)
{
    return
        TokenHasKind::getDebug(
            format(
                ", value=%s",
                getValueString().c_str()
            ) + inside
        );
}