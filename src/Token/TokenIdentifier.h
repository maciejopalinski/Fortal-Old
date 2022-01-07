#pragma once
#include "Token.h"

class TokenIdentifier : public Token
{
    private:
        string name;

    public:
        TokenIdentifier(string name = "");

        TokenType getType() override
        {
            return TOKEN_IDENTIFIER;
        }

        string getName();
        void setName(string name);

        string getDebug(const string &inside = "") override;
};