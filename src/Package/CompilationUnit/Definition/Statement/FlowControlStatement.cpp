#include "FlowControlStatement.h"

FlowControlStatement::FlowControlStatement(StatementFlowControlType kind) : StatementHasKind(kind)
{
}

string FlowControlStatement::getPrintableKindString(StatementFlowControlType kind)
{
    return TokenKeyword::lexer_matches[kind];
}

string FlowControlStatement::getPrintableKindString()
{
    return getPrintableKindString(getKind());
}