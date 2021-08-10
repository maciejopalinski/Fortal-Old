#pragma once
#include "../TokenType/TokenType.h"

class TokenIdentifier
{
    public:
        char *name;

        TokenIdentifier(char *name, size_t size);
        ~TokenIdentifier();

        void debug(Location *location);
};