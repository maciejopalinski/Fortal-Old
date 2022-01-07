#pragma once

#include "Statement.h"
#include "../Expression/Expression.h"

class ExpressionStatement : public Statement
{
    private:
        shared_ptr<Expression> expression;

    public:
        ExpressionStatement(shared_ptr<Expression> expression);

        StatementType getType()
        {
            return STATEMENT_EXPRESSION;
        }

        shared_ptr<Expression> getExpression();
        void setExpression(shared_ptr<Expression> expression);

        string getDebug(string indent, bool last = true, const string &custom = "") override;
};