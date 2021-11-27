#pragma once
#include "TokenHasType.hpp"

enum TokenBracketType
{
    TOKEN_BRACKET_PAREN_L,
    TOKEN_BRACKET_PAREN_R,
    TOKEN_BRACKET_SQUARE_L,
    TOKEN_BRACKET_SQUARE_R,
    TOKEN_BRACKET_CURLY_L,
    TOKEN_BRACKET_CURLY_R
};

const char *TokenBracketType_strings[] = {
    "TOKEN_BRACKET_PAREN_L",
    "TOKEN_BRACKET_PAREN_R",
    "TOKEN_BRACKET_SQUARE_L",
    "TOKEN_BRACKET_SQUARE_R",
    "TOKEN_BRACKET_CURLY_L",
    "TOKEN_BRACKET_CURLY_R"
};

class TokenBracket : public TokenHasType<TokenBracketType>
{
    protected:
        static vector<char> lexer_matches;

    public:
        TokenBracket(TokenBracketType type) : TokenHasType(type) {}

        TokenType getTokenType() { return TOKEN_BRACKET; }
        const char *getTokenTypeString() { return "TokenBracket"; }
        string getTypeString(TokenBracketType type) { return TokenBracketType_strings[type]; }

        string getDebug()
        {
            return format(
                "%s(type='%s')",
                getTokenTypeString(),
                getTypeString(getType()).c_str()
            );
        }

        static vector<char> getLexerMatches() { return lexer_matches; }
        static bool getLexerWhitespaceAfter() { return false; }
        static TokenBracketType getLexerTypeFromMatch(char match)
        {
            auto it = find(lexer_matches.begin(), lexer_matches.end(), match);
            if (it != lexer_matches.end())
            {
                size_t index = it - lexer_matches.begin();
                return (TokenBracketType) index;
            }
            else return (TokenBracketType) -1;
        }
};

vector<char> TokenBracket::lexer_matches = {
    '(', ')', '[', ']', '{', '}'
};