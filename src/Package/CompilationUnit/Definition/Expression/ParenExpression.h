#pragma once

#include "Expression.h"

class ParenExpression : public Expression
{
    private:
        shared_ptr<Expression> expression;

    public:
        ParenExpression(shared_ptr<Expression> expression);

        ExpressionType getType() override
        {
            return EXPRESSION_PAREN;
        }

        shared_ptr<Expression> getExpression();
        void setExpression(shared_ptr<Expression> expression);

        string getDebug(string indent, bool last = true, const string &custom = "") override;
};