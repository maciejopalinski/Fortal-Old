#pragma once

enum ModifierAccessType
{
    MODIFIER_ACCESS_DEFAULT,
    MODIFIER_ACCESS_PUBLIC,
    MODIFIER_ACCESS_PROTECTED,
    MODIFIER_ACCESS_PRIVATE
};

enum ModifierLinkerType
{
    MODIFIER_LINKER_STATIC,
    MODIFIER_LINKER_NATIVE,
    MODIFIER_LINKER_ENTRYPOINT
};

enum ModifierInheritanceType
{
    MODIFIER_INHERITANCE_ABSTRACT,
    MODIFIER_INHERITANCE_FINAL
};

class Modifiers
{
    private:
        ModifierAccessType access_modifier = MODIFIER_ACCESS_DEFAULT;

        bool isStatic = false;
        bool isNative = false;
        bool isEntrypoint = false;

        bool isAbstract = false;
        bool isFinal = false;

    public:
        void setAccess(ModifierAccessType mod);
        ModifierAccessType getAccess();

        void setStatic(bool val);
        bool getStatic();

        void setNative(bool val);
        bool getNative();

        void setEntrypoint(bool val);
        bool getEntrypoint();

        void setAbstract(bool val);
        bool getAbstract();

        void setFinal(bool val);
        bool getFinal();
};