#include "LoopStatement.h"

const char *StatementLoopType_printable_strings[] =
{
    "For",
    "While",
    "DoWhile"
};

string LoopStatement::getPrintableKindString(StatementLoopType kind)
{
    auto idx = magic_enum::enum_index<StatementLoopType>(kind);
    if (idx.has_value())
    {
        return StatementLoopType_printable_strings[idx.value()];
    }
    return "(null)";
}

string LoopStatement::getPrintableKindString()
{
    return getPrintableKindString(getKind());
}

shared_ptr<Expression> LoopStatement::getForInit()
{
    return for_init;
}

void LoopStatement::setForInit(shared_ptr<Expression> for_init)
{
    this->for_init = for_init;
}

shared_ptr<Expression> LoopStatement::getForIter()
{
    return for_iter;
}

void LoopStatement::setForIter(shared_ptr<Expression> for_iter)
{
    this->for_iter = for_iter;
}