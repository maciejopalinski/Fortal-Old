#include "IdentifierExpression.h"

IdentifierExpression::IdentifierExpression(shared_ptr<TokenIdentifier> identifier)
{
    setIdentifier(identifier);
}

shared_ptr<TokenIdentifier> IdentifierExpression::getIdentifier()
{
    return identifier;
}

void IdentifierExpression::setIdentifier(shared_ptr<TokenIdentifier> identifier)
{
    this->identifier = identifier;
}

string IdentifierExpression::getDebug(string indent, bool last, const string &custom)
{
    string output = Expression::getDebug(indent, last, identifier->getName() + custom);

    return output;
}