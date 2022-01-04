#pragma once

#include "Expression.h"
#include "../../../../Token/TokenLiteral.h"

class LiteralExpression : public Expression
{
    private:
        shared_ptr<TokenLiteral> literal;

    public:
        LiteralExpression(shared_ptr<TokenLiteral> literal);

        ExpressionType getType()
        {
            return EXPRESSION_LITERAL;
        }

        shared_ptr<TokenLiteral> getLiteral();
        void setLiteral(shared_ptr<TokenLiteral> literal);

        string getDebug(string indent, bool last = true, string custom = "");
};