#pragma once
#include <vector>
using std::vector;

#include "Definition.h"
#include "VariableDefinition.h"
#include "DataType/DataType.h"

class FunctionDefinition : public Definition
{
    private:
        shared_ptr<DataType> return_type;
        vector<shared_ptr<VariableDefinition>> parameters;

    public:
        FunctionDefinition(shared_ptr<Modifiers> modifiers, shared_ptr<DataType> return_type, shared_ptr<TokenIdentifier> identifier);

        DefinitionType getType()
        {
            return DEFINITION_FUNCTION;
        }

        shared_ptr<DataType> getReturnType();

        void addParameter(shared_ptr<VariableDefinition> definition);
        vector<shared_ptr<VariableDefinition>> getParameters();

        string getDebug(string indent, bool last = true, string custom = "");
};