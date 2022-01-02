#include "ClassDefinition.h"

ClassDefinition::ClassDefinition(shared_ptr<TokenIdentifier> identifier) : Definition(identifier) {}

shared_ptr<Definition> ClassDefinition::addDefinition(shared_ptr<Definition> definition)
{
    members.push_back(definition);
    return definition;
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

string ClassDefinition::getDebug(string indent, bool last, string custom)
{
    custom = getIdentifier()->getName();

    if (!extends.empty())
    {
        custom += " extends ";
        for (size_t i = 0; i < extends.size(); i++)
        {
            auto ident = extends[i];
            custom += ident->getName();

            bool isLast = (i + 1 == extends.size());
            if (!isLast) custom += ", ";
        }
    }

    string output = Definition::getDebug(indent, last, custom);

    if (last)
    {
        indent += "    ";
    }
    else
    {
        indent += "│   ";
    }

    for (size_t i = 0; i < members.size(); i++)
    {
        bool isLast = (i + 1 == members.size());

        auto member = members[i];
        output += member->getDebug(indent, isLast);
    }

    return output;
}