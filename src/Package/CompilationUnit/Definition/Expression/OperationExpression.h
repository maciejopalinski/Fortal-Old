#pragma once

#include "ExpressionHasKind.h"
#include "../../../../Token/TokenIdentifier.h"
#include "../../../../Token/TokenOperator.h"

enum ExpressionOperationType
{
    EXPRESSION_OPERATION_INVALID,
    EXPRESSION_OPERATION_MEMBER_ACCESS,
    EXPRESSION_OPERATION_CALL,
    EXPRESSION_OPERATION_INDEX,
    EXPRESSION_OPERATION_BINARY, // <Expression> <Operator> <Expression>
    EXPRESSION_OPERATION_UNARY, // <Operator><Expression>
    EXPRESSION_OPERATION_PREFIX, // <Operator><Expression>
    EXPRESSION_OPERATION_POSTFIX, // <Expression><Operator>
    EXPRESSION_OPERATION_TERNARY
};

extern const char *OperationExpressionType_printable_strings[];

class OperationExpression : public ExpressionHasKind<ExpressionOperationType>
{
    private:
        vector<shared_ptr<Expression>> call_args;
        shared_ptr<TokenOperator> t_operator;
        shared_ptr<Expression> expr_condition;
        shared_ptr<Expression> expr_left;
        shared_ptr<Expression> expr_right;

    public:
        OperationExpression(ExpressionOperationType kind = EXPRESSION_OPERATION_INVALID);
        virtual ~OperationExpression();

        ExpressionType getType() override
        {
            return EXPRESSION_OPERATION;
        }

        shared_ptr<TokenOperator> getOperator();
        shared_ptr<Expression> getCondition();
        shared_ptr<Expression> getLeftExpression();
        shared_ptr<Expression> getRightExpression();
        void setOperator(shared_ptr<TokenOperator> t_operator);
        void setCondition(shared_ptr<Expression> expr_condition);
        void setLeftExpression(shared_ptr<Expression> expr_left);
        void setRightExpression(shared_ptr<Expression> expr_right);

        vector<shared_ptr<Expression>> getCallArgs();
        void addCallArgs(vector<shared_ptr<Expression>> call_args);
        void addCallArg(shared_ptr<Expression> call_arg);

        static string getPrintableKindString(ExpressionOperationType kind);
        string getPrintableKindString() override;

        string getDebug(string indent, bool last = true, const string &custom = "") override;
};