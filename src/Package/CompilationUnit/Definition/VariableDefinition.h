#pragma once

#include "Definition.h"
#include "DataType/DataType.h"
#include "Expression/Expression.h"

class VariableDefinition : public Definition
{
    private:
        shared_ptr<DataType> data_type;
        shared_ptr<Expression> default_value = nullptr;

    public:
        VariableDefinition(shared_ptr<DataType> data_type, shared_ptr<TokenIdentifier> identifier);

        DefinitionType getType()
        {
            return DEFINITION_VARIABLE;
        }

        shared_ptr<DataType> getDataType();

        void setDefaultValue(shared_ptr<Expression> default_value);
        shared_ptr<Expression> getDefaultValue();

        string getDebug(string indent, bool last = true, const string &custom = "") override;
        string getFunctionParamDebug(string indent, bool last = false, bool simple = true);
};