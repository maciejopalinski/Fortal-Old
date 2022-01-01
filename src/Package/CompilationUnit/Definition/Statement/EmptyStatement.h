#pragma once

#include "Statement.h"

class EmptyStatement : public Statement
{
    public:
        StatementType getType()
        {
            return STATEMENT_EMPTY;
        }
};