#pragma once
#include "TokenHasType.hpp"

enum TokenCommentType
{
    TOKEN_COMMENT_LINE,
    TOKEN_COMMENT_BLOCK
};

const char *TokenCommentType_strings[] = { "TOKEN_COMMENT_LINE", "TOKEN_COMMENT_BLOCK" };

class TokenComment : public TokenHasType<TokenCommentType>
{
    private:
    string content;

    public:
    TokenComment(TokenCommentType type = TOKEN_COMMENT_BLOCK, string content = "")
        : TokenHasType(type)
    {
        setContent(content);
    };

    TokenType getTokenType() { return TOKEN_COMMENT; }
    const char *getTokenTypeString() { return "TokenComment"; }
    string getTypeString(TokenCommentType type) { return TokenCommentType_strings[type]; }

    string getContent() { return this->content; }
    void setContent(const string content) { this->content = content; }

    string getDebug()
    {
        return format(
            "%s(type='%s', content='%s')",
            getTokenTypeString(),
            getTypeString(getType()).c_str(),
            content.c_str());
    }
};