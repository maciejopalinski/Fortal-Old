#include "VariableDefinitionExpression.h"

VariableDefinitionExpression::VariableDefinitionExpression(shared_ptr<VariableDefinition> definition)
{
    setDefinition(definition);
}

shared_ptr<VariableDefinition> VariableDefinitionExpression::getDefinition()
{
    return definition;
}

void VariableDefinitionExpression::setDefinition(shared_ptr<VariableDefinition> definition)
{
    this->definition = definition;
}

string VariableDefinitionExpression::getDebug(string indent, bool last, string custom)
{
    string output = Expression::getDebug(indent, last, custom);

    if (last)
    {
        indent += "    ";
    }
    else
    {
        indent += "│   ";
    }

    output += definition->getDebug(indent);

    return output;
}
