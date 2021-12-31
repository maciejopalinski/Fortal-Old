#pragma once
#include "TokenHasKind.h"

enum TokenCommentKind
{
    TOKEN_COMMENT_LINE,
    TOKEN_COMMENT_BLOCK
};

extern const char *TokenCommentType_printable_strings[];

class TokenComment : public TokenHasKind<TokenCommentKind>
{
    private:
        string content;

    public:
        TokenComment(TokenCommentKind kind, string content = "");

        TokenType getType()
        {
            return TOKEN_COMMENT;
        }

        string getContent();
        void setContent(const string content);

        static string getPrintableKindString(TokenCommentKind kind);
        string getPrintableKindString();

        string getDebug(string inside = "");
};