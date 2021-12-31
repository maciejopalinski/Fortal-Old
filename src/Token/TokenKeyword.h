#pragma once
#include "TokenHasKind.h"

enum TokenKeywordType
{
    TOKEN_KEYWORD_PACKAGE,
    TOKEN_KEYWORD_EXTENDS,
    TOKEN_KEYWORD_IMPORT,
    TOKEN_KEYWORD_STATIC,
    TOKEN_KEYWORD_ABSTRACT,
    TOKEN_KEYWORD_FINAL,
    TOKEN_KEYWORD_ENTRYPOINT,
    TOKEN_KEYWORD_NATIVE,
    TOKEN_KEYWORD_PUBLIC,
    TOKEN_KEYWORD_PROTECTED,
    TOKEN_KEYWORD_PRIVATE,
    TOKEN_KEYWORD_ALIAS,
    TOKEN_KEYWORD_CLASS,
    TOKEN_KEYWORD_ENUM,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_SWITCH,
    TOKEN_KEYWORD_CASE,
    TOKEN_KEYWORD_DEFAULT,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_DO,
    TOKEN_KEYWORD_BREAK,
    TOKEN_KEYWORD_CONTINUE,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_THROW,
    TOKEN_KEYWORD_TRY,
    TOKEN_KEYWORD_CATCH,
    TOKEN_KEYWORD_FINALLY,
    TOKEN_KEYWORD_VOID,
    TOKEN_KEYWORD_INT1,
    TOKEN_KEYWORD_INT8,
    TOKEN_KEYWORD_INT16,
    TOKEN_KEYWORD_INT32,
    TOKEN_KEYWORD_INT64,
    TOKEN_KEYWORD_UINT8,
    TOKEN_KEYWORD_UINT16,
    TOKEN_KEYWORD_UINT32,
    TOKEN_KEYWORD_UINT64,
    TOKEN_KEYWORD_FLOAT,
    TOKEN_KEYWORD_DOUBLE
};

class TokenKeyword : public TokenHasKind<TokenKeywordType>
{
    public:
        static const vector<string> lexer_matches;
        static const bool lexer_whitespace_after = true;

        TokenKeyword(TokenKeywordType kind);

        TokenType getType()
        {
            return TOKEN_KEYWORD;
        }

        static string getPrintableKindString(TokenKeywordType kind);
        string getPrintableKindString();

        static TokenKeywordType getLexerTypeFromMatch(string match);
};