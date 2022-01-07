#pragma once
#include <memory>
using std::shared_ptr;
using std::make_shared;

#include "../../../Token/TokenIdentifier.h"
#include "Modifiers/Modifiers.h"

enum DefinitionType
{
    DEFINITION_CLASS,
    DEFINITION_FUNCTION,
    DEFINITION_VARIABLE,
    DEFINITION_ENUM,
    DEFINITION_ALIAS
};

extern const char *DefinitionType_printable_strings[];

class Definition
{
    private:
        shared_ptr<Modifiers> modifiers;
        shared_ptr<TokenIdentifier> identifier;

    public:
        Definition(shared_ptr<TokenIdentifier> identifier);
        virtual ~Definition();

        virtual DefinitionType getType() = 0;

        static const char *getTypeString(DefinitionType type);
        const char *getTypeString();

        static const char *getPrintableTypeString(DefinitionType type);
        const char *getPrintableTypeString();

        shared_ptr<Modifiers> getModifiers();
        void setModifiers(shared_ptr<Modifiers> modifiers);

        shared_ptr<TokenIdentifier> getIdentifier();
        void setIdentifier(shared_ptr<TokenIdentifier> identifier);

        virtual string getDebug(string indent, bool last = true, const string &custom = "");
};