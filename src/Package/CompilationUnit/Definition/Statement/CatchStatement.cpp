#include "CatchStatement.h"

shared_ptr<VariableDefinition> CatchStatement::getCatchError()
{
    return catch_error;
}

void CatchStatement::setCatchError(shared_ptr<VariableDefinition> catch_error)
{
    this->catch_error = catch_error;
}