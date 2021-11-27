#pragma once
#include "TokenBase.hpp"

class TokenIdentifier : public TokenBase
{
    private:
    string name;

    public:
    TokenIdentifier(string name) { this->setName(name); }

    TokenType getTokenType() { return TOKEN_IDENTIFIER; }
    const char *getTokenTypeString() { return "TokenIdentifier"; }

    string getName() { return this->name; }
    void setName(string name) { this->name = name; }

    string getDebug() { return format("%s(name='%s')", getTokenTypeString(), name.c_str()); }
};