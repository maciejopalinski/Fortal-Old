#include "TokenComment.h"

const char *TokenCommentType_printable_strings[] =
{
    "line",
    "block"
};

TokenComment::TokenComment(TokenCommentKind kind, const string &content) : TokenHasKind(kind)
{
    setContent(content);
};

string TokenComment::getContent()
{
    return this->content;
}

void TokenComment::setContent(const string &content)
{
    this->content = content;
}

string TokenComment::getPrintableKindString(TokenCommentKind kind)
{
    auto idx = magic_enum::enum_index<TokenCommentKind>(kind);
    if (idx.has_value())
    {
        return TokenCommentType_printable_strings[idx.value()];
    }
    return "(null)";
}

string TokenComment::getPrintableKindString()
{
    return getPrintableKindString(getKind());
}

string TokenComment::getDebug(const string &inside)
{
    return
        TokenHasKind::getDebug(
            format(
                ", content='%s'",
                content.c_str()
            ) + inside
        );
}