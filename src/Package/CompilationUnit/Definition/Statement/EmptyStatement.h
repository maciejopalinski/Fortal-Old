#pragma once

#include "Statement.h"

class EmptyStatement : public Statement
{
    public:
        StatementType getType() override
        {
            return STATEMENT_EMPTY;
        }
};