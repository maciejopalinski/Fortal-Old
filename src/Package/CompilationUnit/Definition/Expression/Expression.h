#pragma once
#include <string>
#include <memory>
using std::string;
using std::shared_ptr;
using std::make_shared;

#include "MagicEnum.hpp"
#include "../../../../Utils/Format.h"

enum ExpressionType
{
    EXPRESSION_LITERAL, // <Literal>
    EXPRESSION_NEW_STATEMENT, // new <Expression>
    EXPRESSION_IDENTIFIER, // <Identifier>
    EXPRESSION_PAREN, // ( <Expression> )
    EXPRESSION_OPERATION
};

extern const char *ExpressionType_printable_strings[];

class Expression
{
    public:
        virtual ~Expression();

        virtual ExpressionType getType() = 0;

        static const char *getTypeString(ExpressionType type);
        const char *getTypeString();

        static const char *getPrintableTypeString(ExpressionType type);
        const char *getPrintableTypeString();

        virtual string getDebug(string indent, bool last = true, string custom = "");
};