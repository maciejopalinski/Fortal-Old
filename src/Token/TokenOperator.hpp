#pragma once
#include "TokenHasType.hpp"

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
    TOKEN_OPERATOR_ACCESS_ADDRESS_OF,
    TOKEN_OPERATOR_ARROW
};

const char *TokenOperatorType_strings[] = {
    "TOKEN_OPERATOR_ASSIGN",
    "TOKEN_OPERATOR_ASSIGN_ADD",
    "TOKEN_OPERATOR_ASSIGN_SUB",
    "TOKEN_OPERATOR_ASSIGN_MULT",
    "TOKEN_OPERATOR_ASSIGN_DIV",
    "TOKEN_OPERATOR_ASSIGN_MOD",
    "TOKEN_OPERATOR_ASSIGN_BIT_AND",
    "TOKEN_OPERATOR_ASSIGN_BIT_OR",
    "TOKEN_OPERATOR_ASSIGN_BIT_XOR",
    "TOKEN_OPERATOR_ASSIGN_BIT_LSHIFT",
    "TOKEN_OPERATOR_ASSIGN_BIT_RSHIFT",
    "TOKEN_OPERATOR_INCR",
    "TOKEN_OPERATOR_DECR",
    "TOKEN_OPERATOR_ARITH_ADD",
    "TOKEN_OPERATOR_ARITH_SUB",
    "TOKEN_OPERATOR_ARITH_MULT",
    "TOKEN_OPERATOR_ARITH_DIV",
    "TOKEN_OPERATOR_ARITH_MOD",
    "TOKEN_OPERATOR_ARITH_BIT_NOT",
    "TOKEN_OPERATOR_ARITH_BIT_AND",
    "TOKEN_OPERATOR_ARITH_BIT_OR",
    "TOKEN_OPERATOR_ARITH_BIT_XOR",
    "TOKEN_OPERATOR_ARITH_BIT_LSHIFT",
    "TOKEN_OPERATOR_ARITH_BIT_RSHIFT",
    "TOKEN_OPERATOR_LOGIC_NOT",
    "TOKEN_OPERATOR_LOGIC_AND",
    "TOKEN_OPERATOR_LOGIC_OR",
    "TOKEN_OPERATOR_CMP_EQ",
    "TOKEN_OPERATOR_CMP_NEQ",
    "TOKEN_OPERATOR_CMP_LESS",
    "TOKEN_OPERATOR_CMP_GREAT",
    "TOKEN_OPERATOR_CMP_LESS_EQ",
    "TOKEN_OPERATOR_CMP_GREAT_EQ",
    "TOKEN_OPERATOR_ACCESS_DEREFERENCE",
    "TOKEN_OPERATOR_ACCESS_ADDRESS_OF",
    "TOKEN_OPERATOR_ARROW"
};

// TODO: fix lexer: `b - 1` is parsed as `IDENTIFIER LITERAL LITERAL`, should be `IDENTIFIER OPERATOR LITERAL`, how is C++ doing it?

class TokenOperator : public TokenHasType<TokenOperatorType>
{
    protected:
        static vector<string> lexer_matches;

    public:
        TokenOperator(TokenOperatorType type) : TokenHasType(type) {}

        TokenType getTokenType() { return TOKEN_OPERATOR; }
        const char *getTokenTypeString() { return "TokenOperator"; }
        string getTypeString(TokenOperatorType type) { return TokenOperatorType_strings[type]; }

        string getDebug()
        {
            return format(
                "%s(type='%s')",
                getTokenTypeString(),
                getTypeString(getType()).c_str()
            );
        }

        static vector<string> getLexerMatches() { return lexer_matches; }
        static bool getLexerWhitespaceAfter() { return false; }
        static TokenOperatorType getLexerTypeFromMatch(string match)
        {
            auto it = find(lexer_matches.begin(), lexer_matches.end(), match);
            if (it != lexer_matches.end())
            {
                size_t index = it - lexer_matches.begin();
                return (TokenOperatorType) index;
            }
            else return (TokenOperatorType) -1;
        }
};

vector<string> TokenOperator::lexer_matches = {
    "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "++", "--", "+", "-", "*", "/", "%", "~", "&", "|", "^", "<<", ">>", "!", "&&", "||", "==", "!=", "<", ">", "<=", ">=", "*", "&", "=>"
};