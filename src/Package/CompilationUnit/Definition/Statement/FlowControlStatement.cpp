#include "FlowControlStatement.h"

FlowControlStatement::FlowControlStatement(StatementFlowControlType kind) : StatementHasKind(kind)
{
}

shared_ptr<Expression> FlowControlStatement::getExpression()
{
    return expression;
}

void FlowControlStatement::setExpression(shared_ptr<Expression> expression)
{
    this->expression = expression;
}

string FlowControlStatement::getPrintableKindString(StatementFlowControlType kind)
{
    return TokenKeyword::lexer_matches[kind];
}

string FlowControlStatement::getPrintableKindString()
{
    return getPrintableKindString(getKind());
}

string FlowControlStatement::getDebug(string indent, bool last, const string &custom)
{
    string output = Statement::getDebug(indent, last, getPrintableKindString() + custom);

    if (last)
    {
        indent += "    ";
    }
    else
    {
        indent += "│   ";
    }

    if (expression)
    {
        output += expression->getDebug(indent);
    }

    return output;
}