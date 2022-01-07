#include "TokenIdentifier.h"

TokenIdentifier::TokenIdentifier(string name)
{
    this->setName(name);
}

string TokenIdentifier::getName()
{
    return this->name;
}

void TokenIdentifier::setName(string name)
{
    this->name = name;
}

string TokenIdentifier::getDebug(const string &inside)
{
    return
        Token::getDebug(
            format(
                "name='%s'",
                name.c_str()
            ) + inside
        );
}