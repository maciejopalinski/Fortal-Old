#pragma once

#include "Statement.h"
#include "../VariableDefinition.h"

class VariableDefinitionStatement : public Statement
{
    private:
        shared_ptr<VariableDefinition> definition;

    public:
        VariableDefinitionStatement(shared_ptr<VariableDefinition> definition);

        StatementType getType()
        {
            return STATEMENT_VARIABLE_DEFINITION;
        }

        shared_ptr<VariableDefinition> getDefinition();
        void setDefinition(shared_ptr<VariableDefinition> definition);

        string getDebug(string indent, bool last = true, const string &custom = "") override;
};