#include "Definition.h"
#include "../../../Utils/MagicEnum.hpp"

const char *DefinitionType_printable_strings[] =
{
    "Class",
    "Function",
    "Variable",
    "Enum",
    "Alias"
};

Definition::Definition(shared_ptr<Modifiers> modifiers, shared_ptr<TokenIdentifier> identifier)
{
    this->modifiers = modifiers;
    this->identifier = identifier;
}

Definition::~Definition()
{
}

const char *Definition::getTypeString(DefinitionType type)
{
    auto name = magic_enum::enum_name<DefinitionType>(type);
    return name.data();
}

const char *Definition::getTypeString()
{
    return getTypeString(getType());
}

const char *Definition::getPrintableTypeString(DefinitionType type)
{
    auto idx = magic_enum::enum_index<DefinitionType>(type);
    if (idx.has_value())
    {
        return DefinitionType_printable_strings[idx.value()];
    }
    return "(null)";
}

const char *Definition::getPrintableTypeString()
{
    return getPrintableTypeString(getType());
}

shared_ptr<TokenIdentifier> Definition::getIdentifier()
{
    return identifier;
}

void Definition::setIdentifier(shared_ptr<TokenIdentifier> identifier)
{
    this->identifier = identifier;
}

string Definition::getDebug(string indent, bool last, string custom)
{
    string output = indent;

    if (custom.empty())
    {
        custom = getIdentifier()->getName();
    }

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