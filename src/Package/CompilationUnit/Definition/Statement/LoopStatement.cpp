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

shared_ptr<Statement> LoopStatement::getForInit()
{
    return for_init;
}

void LoopStatement::setForInit(shared_ptr<Statement> for_init)
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

string LoopStatement::getDebug(string indent, bool last, string custom)
{
    string output = Statement::getDebug(indent, last, this->getKindString() + custom);

    if (last)
    {
        indent += "    ";
    }
    else
    {
        indent += "│   ";
    }

    if (for_init)
    {
        bool last = !(getCondition() || for_iter || getBody());
        output += for_init->getDebug(indent, last);
    }
    if (getCondition())
    {
        bool last = !(for_iter || getBody());
        output += getCondition()->getDebug(indent, last);
    }
    if (for_iter)
    {
        bool last = !getBody();
        output += for_iter->getDebug(indent, last);
    }
    if (getBody())
    {
        output += getBody()->getDebug(indent);
    }

    return output;
}
