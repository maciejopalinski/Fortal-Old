#include "StatementHasCondition.h"

shared_ptr<Expression> StatementHasCondition::getCondition()
{
    return condition;
}

void StatementHasCondition::setCondition(shared_ptr<Expression> condition)
{
    this->condition = condition;
}