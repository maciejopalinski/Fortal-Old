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

vector<shared_ptr<TokenIdentifier>> ClassDefinition::getExtends()
{
    return extends;
}

void ClassDefinition::addExtends(vector<shared_ptr<TokenIdentifier>> extends)
{
    for (auto ext : extends)
    {
        addExtend(ext);
    }
}

void ClassDefinition::addExtend(shared_ptr<TokenIdentifier> extend)
{
    extends.push_back(extend);
}