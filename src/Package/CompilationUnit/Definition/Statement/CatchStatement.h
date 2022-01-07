#pragma once

#include "StatementHasBody.h"
#include "../VariableDefinition.h"

class CatchStatement : public StatementHasBody
{
    private:
        shared_ptr<VariableDefinition> catch_error;

    public:
        StatementType getType() override
        {
            return STATEMENT_CATCH;
        }

        shared_ptr<VariableDefinition> getCatchError();
        void setCatchError(shared_ptr<VariableDefinition> catch_error);
};