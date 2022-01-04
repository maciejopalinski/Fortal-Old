#pragma once

#include "StatementHasCondition.h"
#include "StatementHasBody.h"

class IfStatement : public StatementHasCondition, public StatementHasBody
{
    private:
        shared_ptr<Statement> else_body;

    public:
        StatementType getType()
        {
            return STATEMENT_IF;
        }

        shared_ptr<Statement> getElseBody();
        void setElseBody(shared_ptr<Statement> else_body);

        shared_ptr<BlockStatement> getElseBody();
        void setElseBody(shared_ptr<BlockStatement> else_body);
};