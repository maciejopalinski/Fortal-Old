#include "TokenHasKind.h"
#include "TokenComment.h"
#include "TokenKeyword.h"
#include "TokenLiteral.h"
#include "TokenSeparator.h"
#include "TokenOperator.h"

template<>
string TokenHasKind<TokenCommentKind>::getDescription(TokenCommentKind kind)
{
    TokenComment token(kind);
    return token.getDescription();
}

template<>
string TokenHasKind<TokenKeywordType>::getDescription(TokenKeywordType kind)
{
    TokenKeyword token(kind);
    return token.getDescription();
}

template<>
string TokenHasKind<TokenLiteralType>::getDescription(TokenLiteralType kind)
{
    TokenLiteral token(kind);
    return token.getDescription();
}

template<>
string TokenHasKind<TokenSeparatorType>::getDescription(TokenSeparatorType kind)
{
    TokenSeparator token(kind);
    return token.getDescription();
}

template<>
string TokenHasKind<TokenOperatorType>::getDescription(TokenOperatorType kind)
{
    TokenOperator token(kind);
    return token.getDescription();
}