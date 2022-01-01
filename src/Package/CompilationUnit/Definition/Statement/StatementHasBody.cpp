#include "StatementHasBody.h"

shared_ptr<Statement> StatementHasBody::getBody()
{
    return body;
}

void StatementHasBody::setBody(shared_ptr<Statement> body)
{
    this->body = body;
}

string StatementHasBody::getDebug(string indent, bool last, string custom)
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

    output += getBody()->getDebug(indent);

    return output;
}