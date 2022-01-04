#include "OperationExpression.h"

const char *OperationExpressionType_printable_strings[] =
{
    "(null)",
    "MemberAccess",
    "FunctionCall",
    "Index",
    "Binary",
    "Unary",
    "Prefix",
    "Postfix",
    "Ternary"
};

OperationExpression::OperationExpression(ExpressionOperationType kind) : ExpressionHasKind(kind) {}

OperationExpression::~OperationExpression() {}

shared_ptr<TokenOperator> OperationExpression::getOperator()
{
    return t_operator;
}

shared_ptr<Expression> OperationExpression::getCondition()
{
    return expr_condition;
}

shared_ptr<Expression> OperationExpression::getLeftExpression()
{
    return expr_left;
}

shared_ptr<Expression> OperationExpression::getRightExpression()
{
    return expr_right;
}

void OperationExpression::setOperator(shared_ptr<TokenOperator> t_operator)
{
    this->t_operator = t_operator;
}

void OperationExpression::setCondition(shared_ptr<Expression> expr_condition)
{
    this->expr_condition = expr_condition;
}

void OperationExpression::setLeftExpression(shared_ptr<Expression> expr_left)
{
    this->expr_left = expr_left;
}

void OperationExpression::setRightExpression(shared_ptr<Expression> expr_right)
{
    this->expr_right = expr_right;
}

vector<shared_ptr<Expression>> OperationExpression::getCallArgs()
{
    return call_args;
}

void OperationExpression::addCallArgs(vector<shared_ptr<Expression>> call_args)
{
    for (auto c : call_args)
    {
        addCallArg(c);
    }
}

void OperationExpression::addCallArg(shared_ptr<Expression> call_arg)
{
    call_args.push_back(call_arg);
}

string OperationExpression::getPrintableKindString(ExpressionOperationType kind)
{
    auto idx = magic_enum::enum_index<ExpressionOperationType>(kind);
    if (idx.has_value())
    {
        return OperationExpressionType_printable_strings[idx.value()];
    }
    return "(null)";
}

string OperationExpression::getPrintableKindString()
{
    return getPrintableKindString(getKind());
}

string OperationExpression::getDebug(string indent, bool last, string custom)
{
    if (t_operator)
    {
        custom += " ";
        custom += t_operator->getPrintableKindString();
    }

    string output = Expression::getDebug(indent, last, getPrintableKindString() + custom);

    if (last)
    {
        indent += "    ";
    }
    else
    {
        indent += "│   ";
    }

    if (expr_condition)
    {
        bool last = !(expr_left || expr_right || !call_args.empty());
        output += expr_condition->getDebug(indent, last);
    }
    if (expr_left)
    {
        bool last = !(expr_right || !call_args.empty());
        output += expr_left->getDebug(indent, last);
    }
    if (expr_right)
    {
        bool last = call_args.empty();
        output += expr_right->getDebug(indent, last);
    }
    if (!call_args.empty())
    {
        for (size_t i = 0; i < call_args.size(); i++)
        {
            bool isLast = (i + 1 == call_args.size());

            auto arg = call_args[i];
            output += arg->getDebug(indent, isLast);
        }
    }

    return output;
}