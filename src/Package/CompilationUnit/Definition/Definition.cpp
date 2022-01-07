#include "Definition.h"
#include "MagicEnum.hpp"

const char *DefinitionType_printable_strings[] =
{
    "Class",
    "Function",
    "Variable",
    "Enum",
    "Alias"
};

Definition::Definition(shared_ptr<TokenIdentifier> identifier)
{
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

shared_ptr<Modifiers> Definition::getModifiers()
{
    return modifiers;
}

void Definition::setModifiers(shared_ptr<Modifiers> modifiers)
{
    this->modifiers = modifiers;
}

shared_ptr<TokenIdentifier> Definition::getIdentifier()
{
    return identifier;
}

void Definition::setIdentifier(shared_ptr<TokenIdentifier> identifier)
{
    this->identifier = identifier;
}

string Definition::getDebug(string indent, bool last, const string &c)
{
    string output = indent;

    string custom = c;
    if (c.empty())
    {
        custom = getIdentifier()->getName();
    }

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
            "[Definition] [%s] ",
            getPrintableTypeString()
        ) + custom;

    if (output[output.size() - 1] != '\n')
    {
        output += "\n";
    }

    return output;
}