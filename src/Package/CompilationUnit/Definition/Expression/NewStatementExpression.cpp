#include "NewStatementExpression.h"

NewStatementExpression::NewStatementExpression(shared_ptr<Expression> expression)
{
    setExpression(expression);
}

shared_ptr<Expression> NewStatementExpression::getExpression()
{
    return expression;
}

void NewStatementExpression::setExpression(shared_ptr<Expression> expression)
{
    this->expression = expression;
}

string NewStatementExpression::getDebug(string indent, bool last, const string &custom)
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
