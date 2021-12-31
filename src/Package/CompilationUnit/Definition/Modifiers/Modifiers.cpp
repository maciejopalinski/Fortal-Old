#include "Modifiers.h"

void Modifiers::setAccess(ModifierAccessType mod)
{
    this->access_modifier = mod;
}
ModifierAccessType Modifiers::getAccess()
{
    return access_modifier;
}

void Modifiers::setStatic(bool val)
{
    this->isStatic = val;
}
bool Modifiers::getStatic()
{
    return isStatic;
}

void Modifiers::setNative(bool val)
{
    this->isNative = val;
}
bool Modifiers::getNative()
{
    return isNative;
}

void Modifiers::setEntrypoint(bool val)
{
    this->isEntrypoint = val;
}
bool Modifiers::getEntrypoint()
{
    return isEntrypoint;
}

void Modifiers::setAbstract(bool val)
{
    this->isAbstract = val;
}
bool Modifiers::getAbstract()
{
    return isAbstract;
}

void Modifiers::setFinal(bool val)
{
    this->isFinal = val;
}
bool Modifiers::getFinal()
{
    return isFinal;
}