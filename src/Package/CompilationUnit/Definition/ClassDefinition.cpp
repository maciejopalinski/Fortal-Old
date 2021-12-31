#include "ClassDefinition.h"

ClassDefinition::ClassDefinition(shared_ptr<Modifiers> modifiers, shared_ptr<TokenIdentifier> identifier) : Definition(modifiers, identifier) {}

shared_ptr<Definition> ClassDefinition::addDefinition(shared_ptr<Definition> definition)
{
    if (definition->getType() == DEFINITION_FUNCTION || definition->getType() == DEFINITION_VARIABLE)
    {
        members.push_back(definition);
        return definition;
    }
    return nullptr;
}

vector<shared_ptr<Definition>> ClassDefinition::getDefinitions()
{
    return members;
}