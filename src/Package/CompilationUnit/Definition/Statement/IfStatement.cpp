#include "IfStatement.h"

shared_ptr<Statement> IfStatement::getElseBody()
{
    return else_body;
}

void IfStatement::setElseBody(shared_ptr<Statement> else_body)
{
    this->else_body = else_body;
}