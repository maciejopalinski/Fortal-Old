#include "IfStatement.h"

shared_ptr<BlockStatement> IfStatement::getElseBody()
{
    return else_body;
}

void IfStatement::setElseBody(shared_ptr<BlockStatement> else_body)
{
    this->else_body = else_body;
}