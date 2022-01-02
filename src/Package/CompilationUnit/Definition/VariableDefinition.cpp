#include "VariableDefinition.h"

VariableDefinition::VariableDefinition(shared_ptr<DataType> data_type, shared_ptr<TokenIdentifier> identifier) : Definition(identifier)
{
    this->data_type = data_type;
}

shared_ptr<DataType> VariableDefinition::getDataType()
{
    return data_type;
}

void VariableDefinition::setDefaultValue(shared_ptr<Expression> default_value)
{
    this->default_value = default_value;
}

shared_ptr<Expression> VariableDefinition::getDefaultValue()
{
    return default_value;
}

string VariableDefinition::getDebug(string indent, bool last, string custom)
{
    return Definition::getDebug(indent, last, getFunctionParamDebug(last) + custom);
}

string VariableDefinition::getFunctionParamDebug(bool last)
{
    string output;
    output = data_type->toString();
    output += " ";
    output += getIdentifier()->getName();
    output += (last ? "" : ", ");

    return output;
}