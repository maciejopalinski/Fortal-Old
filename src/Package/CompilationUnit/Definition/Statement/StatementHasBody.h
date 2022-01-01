#pragma once

#include "Statement.h"
#include "BlockStatement.h"

class StatementHasBody : virtual public Statement
{
    private:
        shared_ptr<Statement> body;

    public:
        shared_ptr<Statement> getBody();
        void setBody(shared_ptr<Statement> body);

        virtual string getDebug(string indent, bool last = true, string custom = "");
};