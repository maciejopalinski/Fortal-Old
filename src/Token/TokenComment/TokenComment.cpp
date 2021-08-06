#include "TokenComment.h"

const char *TokenCommentType_strings[] =
{
    "TOKEN_COMMENT_LINE",
    "TOKEN_COMMENT_BLOCK"
};
const char *TokenCommentType_enum_to_string(TokenCommentType index) { return TokenCommentType_strings[index]; }

TokenComment::TokenComment(TokenCommentType type) : type(type) {}

TokenComment::~TokenComment()
{
    // printf("[DEBUG] TokenComment destroyed\n");
    if (content) delete[] content;
}

void TokenComment::debug(const char *prefix)
{
    size_t size = strlen(content);
    char *escaped = new char[4 * size];
    replace_escape_characters(content, size, escaped);

    printf
    (
        "%sTokenComment(type='%s', content='%s')\n",
        prefix,
        TokenCommentType_enum_to_string(type),
        escaped
    );

    delete[] escaped;
}