#pragma once

#include "Definition.h"
#include "DataType/DataType.h"

class AliasDefinition : public Definition
{
    private:
        shared_ptr<DataType> target;

    public:
        AliasDefinition(shared_ptr<TokenIdentifier> identifier, shared_ptr<DataType> target);

        DefinitionType getType() override
        {
            return DEFINITION_ALIAS;
        }

        void setTarget(shared_ptr<DataType> target);
        shared_ptr<DataType> getTarget();

        string getDebug(string indent, bool last = true, const string &custom = "") override;
};