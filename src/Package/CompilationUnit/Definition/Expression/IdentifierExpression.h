#pragma once

#include "Expression.h"
#include "../../../../Token/TokenIdentifier.h"

class IdentifierExpression : public Expression
{
    private:
        shared_ptr<TokenIdentifier> identifier;

    public:
        IdentifierExpression(shared_ptr<TokenIdentifier> identifier);

        ExpressionType getType()
        {
            return EXPRESSION_IDENTIFIER;
        }

        shared_ptr<TokenIdentifier> getIdentifier();
        void setIdentifier(shared_ptr<TokenIdentifier> identifier);

        string getDebug(string indent, bool last = true, const string &custom = "") override;
};