#pragma once

#include "Expression.h"

class NewStatementExpression : public Expression
{
    private:
        shared_ptr<Expression> expression;

    public:
        NewStatementExpression(shared_ptr<Expression> expression);

        ExpressionType getType() override
        {
            return EXPRESSION_NEW_STATEMENT;
        }

        shared_ptr<Expression> getExpression();
        void setExpression(shared_ptr<Expression> expression);

        string getDebug(string indent, bool last = true, const string &custom = "") override;
};