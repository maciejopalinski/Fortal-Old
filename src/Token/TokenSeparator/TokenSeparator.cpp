#include "TokenSeparator.h"

const char separators[] =
{
    '.',
    ',',
    ';',
    ':',
    '?',
    '(',
    ')',
    '[',
    ']',
    '{',
    '}',
    0
};

const char *TokenSeparatorType_strings[] =
{
    "TOKEN_SEPARATOR_DOT",
    "TOKEN_SEPARATOR_COMMA",
    "TOKEN_SEPARATOR_SEMICOLON",
    "TOKEN_SEPARATOR_COLON",
    "TOKEN_SEPARATOR_QUESTION_MARK",
    "TOKEN_SEPARATOR_PAREN_L",
    "TOKEN_SEPARATOR_PAREN_R",
    "TOKEN_SEPARATOR_SQUARE_BRACKET_L",
    "TOKEN_SEPARATOR_SQUARE_BRACKET_R",
    "TOKEN_SEPARATOR_CURLY_BRACKET_L",
    "TOKEN_SEPARATOR_CURLY_BRACKET_R"
};
const char *TokenSeparatorType_enum_to_string(TokenSeparatorType index) { return TokenSeparatorType_strings[index]; }

TokenSeparator::TokenSeparator(TokenSeparatorType type) : type(type) {}

TokenSeparator::~TokenSeparator()
{
    // printf("[DEBUG] TokenSeparator destroyed\n");
}

void TokenSeparator::debug(const char *prefix)
{
    printf
    (
        "%sTokenSeparator(type='%s')\n",
        prefix,
        TokenSeparatorType_enum_to_string(type)
    );
}