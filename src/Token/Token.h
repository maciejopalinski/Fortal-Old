#pragma once
#include "TokenType/TokenType.h"
#include "TokenComment/TokenComment.h"
#include "TokenKeyword/TokenKeyword.h"
#include "TokenSeparator/TokenSeparator.h"
#include "TokenOperator/TokenOperator.h"
#include "TokenLiteral/TokenLiteral.h"
#include "TokenIdentifier/TokenIdentifier.h"

class Token
{
    public:
        TokenType type;

        union
        {
            TokenComment *comment;
            TokenKeyword *keyword;
            TokenSeparator *separator;
            TokenOperator *_operator;
            TokenLiteral *literal;
            TokenIdentifier *identifier;
        };

        Token(TokenType type = TOKEN_NULL);
        ~Token();

        void debug(const char *prefix = "");

        operator bool() const;
        operator TokenType() const;
};