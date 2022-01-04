#include "ParenExpression.h"

ParenExpression::ParenExpression(shared_ptr<Expression> expression)
{
    setExpression(expression);
}

shared_ptr<Expression> ParenExpression::getExpression()
{
    return expression;
}

void ParenExpression::setExpression(shared_ptr<Expression> expression)
{
    this->expression = expression;
}

string ParenExpression::getDebug(string indent, bool last, string custom)
{
    string output = Expression::getDebug(indent, last, custom);

    if (last)
    {
        indent += "    ";
    }
    else
    {
        indent += "│   ";
    }

    output += expression->getDebug(indent);

    return output;
}
