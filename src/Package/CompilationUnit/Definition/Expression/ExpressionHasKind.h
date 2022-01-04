#pragma once

#include "../../../../Utils/MagicEnum.hpp"
#include "Expression.h"

template<class T>
class ExpressionHasKind : public Expression
{
    private:
        T kind;

    public:
        ExpressionHasKind(T kind)
        {
            setKind(kind);
        }

        T getKind()
        {
            return kind;
        }

        void setKind(T kind)
        {
            this->kind = kind;
        }

        const char *getKindString()
        {
            return getKindString(kind);
        }

        static const char *getKindString(T kind)
        {
            auto name = magic_enum::enum_name<T>(kind);
            return name.data();
        }

        virtual string getPrintableKindString() = 0;
};