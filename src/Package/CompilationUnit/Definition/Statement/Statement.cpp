#include "Statement.h"

const char *StatementType_printable_strings[] =
{
    "Expression",
    "Block",
    "If",
    "Switch",
    "Loop",
    "FlowControl",
    "Try",
    "Catch",
    "Empty"
};

Statement::~Statement()
{
}

const char *Statement::getTypeString(StatementType type)
{
    auto name = magic_enum::enum_name<StatementType>(type);
    return name.data();
}

const char *Statement::getTypeString()
{
    return getTypeString(getType());
}

const char *Statement::getPrintableTypeString(StatementType type)
{
    auto idx = magic_enum::enum_index<StatementType>(type);
    if (idx.has_value())
    {
        return StatementType_printable_strings[idx.value()];
    }
    return "(null)";
}

const char *Statement::getPrintableTypeString()
{
    return getPrintableTypeString(getType());
}

string Statement::getDebug(string indent, bool last, string custom)
{
    string output = indent;

    if (last)
    {
        output += "└── ";
        indent += "    ";
    }
    else
    {
        output += "├── ";
        indent += "│   ";
    }

    output +=
        format(
            "[%s] ",
            getPrintableTypeString()
        ) + custom + "\n";

    return output;
}