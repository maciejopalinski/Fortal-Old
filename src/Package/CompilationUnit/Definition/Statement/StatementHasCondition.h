#pragma once

#include "Statement.h"
#include "../Expression/Expression.h"

class StatementHasCondition : virtual public Statement
{
    private:
        shared_ptr<Expression> condition;

    public:
        shared_ptr<Expression> getCondition();
        void setCondition(shared_ptr<Expression> condition);
};