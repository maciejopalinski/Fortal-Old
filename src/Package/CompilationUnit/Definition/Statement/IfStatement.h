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

        string getDebug(string indent, bool last = true, string custom = "");
};