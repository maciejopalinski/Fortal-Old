#pragma once
#include <vector>
using std::vector;

#include "Definition.h"

class ClassDefinition : public Definition
{
    private:
        vector<shared_ptr<Definition>> members;

        vector<shared_ptr<TokenIdentifier>> extends;

    public:
        ClassDefinition(shared_ptr<Modifiers> modifiers, shared_ptr<TokenIdentifier> identifier);

        DefinitionType getType()
        {
            return DEFINITION_CLASS;
        }

        shared_ptr<Definition> addDefinition(shared_ptr<Definition> definition);
        vector<shared_ptr<Definition>> getDefinitions();

        vector<shared_ptr<TokenIdentifier>> getExtends();
        void addExtends(vector<shared_ptr<TokenIdentifier>> extends);
        void addExtend(shared_ptr<TokenIdentifier> extend);
};