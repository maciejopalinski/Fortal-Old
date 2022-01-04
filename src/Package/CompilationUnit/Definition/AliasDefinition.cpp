#include "AliasDefinition.h"

AliasDefinition::AliasDefinition(shared_ptr<TokenIdentifier> identifier, shared_ptr<DataType> target) : Definition(identifier)
{
    setTarget(target);
}


void AliasDefinition::setTarget(shared_ptr<DataType> target)
{
    this->target = target;
}

shared_ptr<DataType> AliasDefinition::getTarget()
{
    return target;
}

string AliasDefinition::getDebug(string indent, bool last, string custom)
{
    return
        Definition::getDebug(
            indent,
            last,
            format(
                "%s = %s",
                getIdentifier()->getName().c_str(),
                target->toString().c_str()
            ) + custom
        );
}