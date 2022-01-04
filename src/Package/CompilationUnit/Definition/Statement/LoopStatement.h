#pragma once

#include "StatementHasKind.h"
#include "StatementHasCondition.h"
#include "StatementHasBody.h"

enum StatementLoopType
{
    STATEMENT_LOOP_FOR,
    STATEMENT_LOOP_WHILE,
    STATEMENT_LOOP_DO_WHILE
};

extern const char *StatementLoopType_printable_strings[];

class LoopStatement : public StatementHasKind<StatementLoopType>, public StatementHasBody, public StatementHasCondition
{
    private:
        shared_ptr<Statement> for_init;
        shared_ptr<Expression> for_iter;

    public:
        LoopStatement(StatementLoopType kind) : StatementHasKind(kind) {}

        StatementType getType()
        {
            return STATEMENT_LOOP;
        }

        static string getPrintableKindString(StatementLoopType kind);
        string getPrintableKindString();

        shared_ptr<Statement> getForInit();
        void setForInit(shared_ptr<Statement> for_init);

        shared_ptr<Expression> getForIter();
        void setForIter(shared_ptr<Expression> for_iter);
};