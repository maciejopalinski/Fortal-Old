#pragma once
#include <vector>
#include <algorithm>
using std::find;
using std::vector;

#include "MagicEnum.hpp"
#include "Token.h"

template<class T>
class TokenHasKind : public Token
{
    private:
        T kind;

    public:
        TokenHasKind(T kind)
        {
            setKind(kind);
        }

        T getKind()
        {
            return this->kind;
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

        virtual string getDebug(string inside = "")
        {
            return
                Token::getDebug(
                    format(
                        "kind='%s'",
                        getKindString()
                    ) + inside
                );
        }

        static string getDescription(T kind);

        virtual string getDescription()
        {
            return
                format(
                    "%s '%s'",
                    Token::getDescription().c_str(),
                    getPrintableKindString().c_str()
                );
        }
};
