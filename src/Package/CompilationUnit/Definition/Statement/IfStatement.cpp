#include "IfStatement.h"

shared_ptr<Statement> IfStatement::getElseBody()
{
    return else_body;
}

void IfStatement::setElseBody(shared_ptr<Statement> else_body)
{
    this->else_body = else_body;
}

string IfStatement::getDebug(string indent, bool last, const string &custom)
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

    if (getCondition())
    {
        last = !(getBody() || else_body);
        output += getCondition()->getDebug(indent, last);
    }

    if (getBody())
    {
        last = !else_body;
        output += getBody()->getDebug(indent, last);
    }

    if (else_body)
    {
        output += else_body->getDebug(indent);
    }

    return output;
}