#pragma once
#include <string>
#include <memory>
using std::string;
using std::shared_ptr;
using std::make_shared;

#include "../../../../Utils/MagicEnum.hpp"
#include "../../../../Utils/Format.h"

enum StatementType
{
    STATEMENT_EXPRESSION,
    STATEMENT_BLOCK,
    STATEMENT_IF,
    STATEMENT_SWITCH,
    STATEMENT_LOOP,
    STATEMENT_FLOW_CONTROL,
    STATEMENT_TRY,
    STATEMENT_CATCH,
    STATEMENT_EMPTY
};

class Statement
{
    public:
        virtual ~Statement();

        virtual StatementType getType() = 0;

        static const char *getTypeString(StatementType type);
        const char *getTypeString();

        static const char *getPrintableTypeString(StatementType type);
        const char *getPrintableTypeString();

        virtual string getDebug(string indent, bool last = true, string custom = "");
};