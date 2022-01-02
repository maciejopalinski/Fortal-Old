#pragma once
#include <vector>
using std::vector;

#include "Definition.h"
#include "VariableDefinition.h"
#include "DataType/DataType.h"
#include "Statement/BlockStatement.h"

class FunctionDefinition : public Definition
{
    private:
        shared_ptr<DataType> return_type;
        vector<shared_ptr<VariableDefinition>> parameters;
        vector<shared_ptr<TokenIdentifier>> throws;

        shared_ptr<BlockStatement> body;

    public:
        FunctionDefinition(shared_ptr<DataType> return_type, shared_ptr<TokenIdentifier> identifier);

        DefinitionType getType()
        {
            return DEFINITION_FUNCTION;
        }

        shared_ptr<DataType> getReturnType();

        void addParameter(shared_ptr<VariableDefinition> definition);
        void addParameters(vector<shared_ptr<VariableDefinition>> definitions);
        vector<shared_ptr<VariableDefinition>> getParameters();

        void addThrow(shared_ptr<TokenIdentifier> ident);
        void addThrows(vector<shared_ptr<TokenIdentifier>> idents);
        vector<shared_ptr<TokenIdentifier>> getThrows();

        shared_ptr<BlockStatement> getBody();
        void setBody(shared_ptr<BlockStatement> body);

        string getDebug(string indent, bool last = true, string custom = "");
};