#pragma once
#include "../TokenType/TokenType.h"

enum TokenCommentType
{
    TOKEN_COMMENT_LINE,
    TOKEN_COMMENT_BLOCK
};
extern const char *TokenCommentType_strings[];
const char *TokenCommentType_enum_to_string(TokenCommentType index);

class TokenComment
{
    public:
        TokenCommentType type;
        char *content;

        TokenComment(TokenCommentType type);
        ~TokenComment();

        void debug(const char *prefix = "");
};