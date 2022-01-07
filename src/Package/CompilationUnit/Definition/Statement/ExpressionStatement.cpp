#include "ExpressionStatement.h"

ExpressionStatement::ExpressionStatement(shared_ptr<Expression> expression)
{
    setExpression(expression);
}

shared_ptr<Expression> ExpressionStatement::getExpression()
{
    return expression;
}

void ExpressionStatement::setExpression(shared_ptr<Expression> expression)
{
    this->expression = expression;
}

string ExpressionStatement::getDebug(string indent, bool last, const string &custom)
{
    string output = Statement::getDebug(indent, last, custom);

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