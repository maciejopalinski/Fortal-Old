#include "VariableDefinitionStatement.h"

VariableDefinitionStatement::VariableDefinitionStatement(shared_ptr<VariableDefinition> definition)
{
    setDefinition(definition);
}

shared_ptr<VariableDefinition> VariableDefinitionStatement::getDefinition()
{
    return definition;
}

void VariableDefinitionStatement::setDefinition(shared_ptr<VariableDefinition> definition)
{
    this->definition = definition;
}

string VariableDefinitionStatement::getDebug(string indent, bool last, const string &custom)
{
    string output = Statement::getDebug(indent, last, custom);

    if (last)
    {
        indent += "    ";
    }
    else
    {
        indent += "│   ";
    }

    if (definition)
    {
        output += definition->getDebug(indent);
    }

    return output;
}