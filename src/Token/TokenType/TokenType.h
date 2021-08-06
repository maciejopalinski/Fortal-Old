#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
using namespace std;

#include "../../Utils/Utils.h"

enum TokenType
{
    TOKEN_NULL,
    TOKEN_COMMENT,
    TOKEN_KEYWORD,
    TOKEN_SEPARATOR,
    TOKEN_OPERATOR,
    TOKEN_LITERAL,
    TOKEN_IDENTIFIER
};

const char *TokenType_enum_to_string(TokenType index);