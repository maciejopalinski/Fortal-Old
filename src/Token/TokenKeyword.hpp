#pragma once
#include "TokenHasType.hpp"

enum TokenKeywordType
{
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_DO,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_BREAK,
    TOKEN_KEYWORD_CONTINUE,
    TOKEN_KEYWORD_IMPORT,
    TOKEN_KEYWORD_VOID,
    TOKEN_KEYWORD_INT
};

const char *TokenKeywordType_strings[] = {
    "TOKEN_KEYWORD_IF",
    "TOKEN_KEYWORD_ELSE",
    "TOKEN_KEYWORD_FOR",
    "TOKEN_KEYWORD_DO",
    "TOKEN_KEYWORD_WHILE",
    "TOKEN_KEYWORD_RETURN",
    "TOKEN_KEYWORD_BREAK",
    "TOKEN_KEYWORD_CONTINUE",
    "TOKEN_KEYWORD_IMPORT",
    "TOKEN_KEYWORD_VOID",
    "TOKEN_KEYWORD_INT"
};

class TokenKeyword : public TokenHasType<TokenKeywordType>
{
    protected:
        static vector<string> lexer_matches;

    public:
        TokenKeyword(TokenKeywordType type) : TokenHasType(type) {}

        TokenType getTokenType() { return TOKEN_KEYWORD; }
        const char *getTokenTypeString() { return "TokenKeyword"; }
        string getTypeString(TokenKeywordType type) { return TokenKeywordType_strings[type]; }

        string getDebug()
        {
            return format(
                "%s(type='%s')",
                getTokenTypeString(),
                getTypeString(getType()).c_str()
            );
        }

        static vector<string> getLexerMatches() { return lexer_matches; }
        static bool getLexerWhitespaceAfter() { return true; }
        static TokenKeywordType getLexerTypeFromMatch(string match)
        {
            auto it = find(lexer_matches.begin(), lexer_matches.end(), match);
            if (it != lexer_matches.end())
            {
                size_t index = it - lexer_matches.begin();
                return (TokenKeywordType) index;
            }
            else return (TokenKeywordType) -1;
        }
};

vector<string> TokenKeyword::lexer_matches = {
    "if", "else", "for", "do", "while", "return", "break", "continue", "import", "void", "int"
};