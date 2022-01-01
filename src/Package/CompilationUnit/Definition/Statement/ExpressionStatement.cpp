#include "ExpressionStatement.h"

shared_ptr<Expression> ExpressionStatement::getExpression()
{
    return expression;
}

void ExpressionStatement::setExpression(shared_ptr<Expression> expression)
{
    this->expression = expression;
}