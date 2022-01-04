#pragma once

#include "Expression.h"
#include "../VariableDefinition.h"

class VariableDefinitionExpression : public Expression
{
    private:
        shared_ptr<VariableDefinition> definition;

    public:
        VariableDefinitionExpression(shared_ptr<VariableDefinition> definition);

        ExpressionType getType()
        {
            return EXPRESSION_VARIABLE_DEFINITION;
        }

        shared_ptr<VariableDefinition> getDefinition();
        void setDefinition(shared_ptr<VariableDefinition> definition);

        string getDebug(string indent, bool last = true, string custom = "");
};