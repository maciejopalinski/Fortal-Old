#pragma once
#include "Token.h"

class TokenIdentifier : public Token
{
    private:
        string name;

    public:
        TokenIdentifier(string name = "");

        TokenType getType()
        {
            return TOKEN_IDENTIFIER;
        }

        string getName();
        void setName(string name);

        string getDebug(string inside = "");
};