#pragma once
#include "../TokenType/TokenType.h"

extern const char *operators[];

enum TokenOperatorType
{
    TOKEN_OPERATOR_ASSIGN,
    TOKEN_OPERATOR_ASSIGN_ADD,
    TOKEN_OPERATOR_ASSIGN_SUB,
    TOKEN_OPERATOR_ASSIGN_MULT,
    TOKEN_OPERATOR_ASSIGN_DIV,
    TOKEN_OPERATOR_ASSIGN_MOD,
    TOKEN_OPERATOR_ASSIGN_BIT_AND,
    TOKEN_OPERATOR_ASSIGN_BIT_OR,
    TOKEN_OPERATOR_ASSIGN_BIT_XOR,
    TOKEN_OPERATOR_ASSIGN_BIT_LSHIFT,
    TOKEN_OPERATOR_ASSIGN_BIT_RSHIFT,
    TOKEN_OPERATOR_INCR,
    TOKEN_OPERATOR_DECR,
    TOKEN_OPERATOR_ARITH_ADD,
    TOKEN_OPERATOR_ARITH_SUB,
    TOKEN_OPERATOR_ARITH_MULT,
    TOKEN_OPERATOR_ARITH_DIV,
    TOKEN_OPERATOR_ARITH_MOD,
    TOKEN_OPERATOR_ARITH_BIT_NOT,
    TOKEN_OPERATOR_ARITH_BIT_AND,
    TOKEN_OPERATOR_ARITH_BIT_OR,
    TOKEN_OPERATOR_ARITH_BIT_XOR,
    TOKEN_OPERATOR_ARITH_BIT_LSHIFT,
    TOKEN_OPERATOR_ARITH_BIT_RSHIFT,
    TOKEN_OPERATOR_LOGIC_NOT,
    TOKEN_OPERATOR_LOGIC_AND,
    TOKEN_OPERATOR_LOGIC_OR,
    TOKEN_OPERATOR_CMP_EQ,
    TOKEN_OPERATOR_CMP_NEQ,
    TOKEN_OPERATOR_CMP_LESS,
    TOKEN_OPERATOR_CMP_GREAT,
    TOKEN_OPERATOR_CMP_LESS_EQ,
    TOKEN_OPERATOR_CMP_GREAT_EQ,
    TOKEN_OPERATOR_ACCESS_DEREFERENCE,
    TOKEN_OPERATOR_ACCESS_ADDRESS_OF
};
extern const char *TokenOperatorType_strings[];
const char *TokenOperatorType_enum_to_string(TokenOperatorType index);

class TokenOperator
{
    public:
        TokenOperatorType type;

        TokenOperator(TokenOperatorType type);
        ~TokenOperator();

        void debug(const char *prefix = "");
};