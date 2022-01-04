#include "LiteralExpression.h"

LiteralExpression::LiteralExpression(shared_ptr<TokenLiteral> literal)
{
    setLiteral(literal);
}

shared_ptr<TokenLiteral> LiteralExpression::getLiteral()
{
    return literal;
}

void LiteralExpression::setLiteral(shared_ptr<TokenLiteral> literal)
{
    this->literal = literal;
}

string LiteralExpression::getDebug(string indent, bool last, string custom)
{
    return Expression::getDebug(indent, last, literal->getValueString() + custom);
}
