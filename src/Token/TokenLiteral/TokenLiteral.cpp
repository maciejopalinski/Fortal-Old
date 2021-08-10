#include "TokenLiteral.h"
#include "../../ErrorHandler/ErrorHandler.h"
#include "../../Utils/Utils.h"

const char *TokenLiteralType_strings[] =
{
    "TOKEN_LITERAL_INT",
    "TOKEN_LITERAL_FLOAT",
    "TOKEN_LITERAL_CHAR",
    "TOKEN_LITERAL_STRING",
    "TOKEN_LITERAL_BOOL",
    "TOKEN_LITERAL_NULLPTR"
};
const char *TokenLiteralType_enum_to_string(TokenLiteralType index) { return TokenLiteralType_strings[index]; }

TokenLiteral::TokenLiteral(long long content)          : type(TOKEN_LITERAL_INT),     literal_int(content) {}
TokenLiteral::TokenLiteral(double content)             : type(TOKEN_LITERAL_FLOAT),   literal_float(content) {}
TokenLiteral::TokenLiteral(char content)               : type(TOKEN_LITERAL_CHAR),    literal_char(content) {}
TokenLiteral::TokenLiteral(bool content)               : type(TOKEN_LITERAL_BOOL),    literal_bool(content) {}
TokenLiteral::TokenLiteral(void *content)              : type(TOKEN_LITERAL_NULLPTR), literal_nullptr(content) {}

TokenLiteral::TokenLiteral(char *content, size_t size) : type(TOKEN_LITERAL_STRING)
{
    this->literal_string = new char[size + 1];
    strcpy(this->literal_string, content);
}

TokenLiteral::~TokenLiteral()
{
    if (type == TOKEN_LITERAL_STRING) delete[] literal_string;
}

void TokenLiteral::debug(Location *location)
{
    if (type == TOKEN_LITERAL_INT)     error_handler.logLocation(E_DEBUG, location, "TokenLiteral(type='%s', value=%lli)\n", TokenLiteralType_enum_to_string(type), literal_int);
    if (type == TOKEN_LITERAL_FLOAT)   error_handler.logLocation(E_DEBUG, location, "TokenLiteral(type='%s', value=%f)\n", TokenLiteralType_enum_to_string(type), literal_float);
    if (type == TOKEN_LITERAL_CHAR)    error_handler.logLocation(E_DEBUG, location, "TokenLiteral(type='%s', value='%c')\n", TokenLiteralType_enum_to_string(type), literal_char);
    if (type == TOKEN_LITERAL_STRING)  error_handler.logLocation(E_DEBUG, location, "TokenLiteral(type='%s', value=\"%s\")\n", TokenLiteralType_enum_to_string(type), literal_string);
    if (type == TOKEN_LITERAL_BOOL)    error_handler.logLocation(E_DEBUG, location, "TokenLiteral(type='%s', value=%s)\n", TokenLiteralType_enum_to_string(type), literal_bool ? "true" : "false");
    if (type == TOKEN_LITERAL_NULLPTR) error_handler.logLocation(E_DEBUG, location, "TokenLiteral(type='%s', value=%s)\n", TokenLiteralType_enum_to_string(type), "nullptr");
}