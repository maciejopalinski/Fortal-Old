#include "TokenComment.h"
#include "../../ErrorHandler/ErrorHandler.h"
#include "../../Utils/Utils.h"

const char *TokenCommentType_strings[] =
{
    "TOKEN_COMMENT_LINE",
    "TOKEN_COMMENT_BLOCK"
};
const char *TokenCommentType_enum_to_string(TokenCommentType index) { return TokenCommentType_strings[index]; }

TokenComment::TokenComment(TokenCommentType type) : type(type) {}

TokenComment::~TokenComment()
{
    if (content) delete[] content;
}

void TokenComment::debug(Location *location)
{
    size_t size = strlen(content);
    char *escaped = new char[4 * size];
    replace_escape_characters(content, size, escaped);

    error_handler.logLocation
    (
        E_DEBUG,
        location,
        "TokenComment(type='%s', content='%s')\n",
        TokenCommentType_enum_to_string(type),
        escaped
    );

    delete[] escaped;
}