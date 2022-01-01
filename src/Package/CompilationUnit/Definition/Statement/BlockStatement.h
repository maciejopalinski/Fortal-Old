#pragma once

#include "Statement.h"

class BlockStatement : public Statement
{
    private:
        vector<shared_ptr<Statement>> statements;

    public:
        StatementType getType()
        {
            return STATEMENT_BLOCK;
        }

        vector<shared_ptr<Statement>> getStatements();
        void addStatement(shared_ptr<Statement> statement);
        void addStatements(vector<shared_ptr<Statement>> statements);

        string getDebug(string indent, bool last = true, string custom = "");
};