#pragma once
#include "../Location/Location.hpp"
#include "TokenType.hpp"

#include <bits/stdc++.h>
using namespace std;

class TokenBase
{
    public:
    Location location;

    virtual ~TokenBase() {}

    virtual TokenType getTokenType()         = 0;
    virtual const char *getTokenTypeString() = 0;

    virtual string getDebug() = 0;
};
