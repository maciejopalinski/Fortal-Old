#include "Expression.h"

const char *ExpressionType_printable_strings[] =
{
    "Literal",
    "NewStatement",
    "Identifier",
    "Paren",
    "Operation"
};

Expression::~Expression()
{
}

const char *Expression::getTypeString(ExpressionType type)
{
    auto name = magic_enum::enum_name<ExpressionType>(type);
    return name.data();
}

const char *Expression::getTypeString()
{
    return getTypeString(getType());
}

const char *Expression::getPrintableTypeString(ExpressionType type)
{
    auto idx = magic_enum::enum_index<ExpressionType>(type);
    if (idx.has_value())
    {
        return ExpressionType_printable_strings[idx.value()];
    }
    return "(null)";
}

const char *Expression::getPrintableTypeString()
{
    return getPrintableTypeString(getType());
}

string Expression::getDebug(string indent, bool last, const string &custom)
{
    string output = indent;

    if (last)
    {
        output += "└── ";
    }
    else
    {
        output += "├── ";
    }

    output +=
        format(
            "[Expression] [%s] ",
            getPrintableTypeString()
        ) + custom + "\n";

    return output;
}