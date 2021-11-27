#pragma once
#include "TokenHasType.hpp"

enum TokenSeparatorType
{
    TOKEN_SEPARATOR_DOT,
    TOKEN_SEPARATOR_COMMA,
    TOKEN_SEPARATOR_SEMICOLON,
    TOKEN_SEPARATOR_COLON,
    TOKEN_SEPARATOR_QUESTION_MARK
};

const char *TokenSeparatorType_strings[] = { "TOKEN_SEPARATOR_DOT",
                                             "TOKEN_SEPARATOR_COMMA",
                                             "TOKEN_SEPARATOR_SEMICOLON",
                                             "TOKEN_SEPARATOR_COLON",
                                             "TOKEN_SEPARATOR_QUESTION_MARK" };

class TokenSeparator : public TokenHasType<TokenSeparatorType>
{
    protected:
    static vector<char> lexer_matches;

    public:
    TokenSeparator(TokenSeparatorType type) : TokenHasType(type) {}

    TokenType getTokenType() { return TOKEN_SEPARATOR; }
    const char *getTokenTypeString() { return "TokenSeparator"; }
    string getTypeString(TokenSeparatorType type) { return TokenSeparatorType_strings[type]; }

    string getDebug()
    {
        return format("%s(type='%s')", getTokenTypeString(), getTypeString(getType()).c_str());
    }

    static vector<char> getLexerMatches() { return lexer_matches; }
    static bool getLexerWhitespaceAfter() { return false; }
    static TokenSeparatorType getLexerTypeFromMatch(char match)
    {
        auto it = find(lexer_matches.begin(), lexer_matches.end(), match);
        if (it != lexer_matches.end())
        {
            size_t index = it - lexer_matches.begin();
            return (TokenSeparatorType) index;
        }
        else return (TokenSeparatorType) -1;
    }
};

vector<char> TokenSeparator::lexer_matches = { '.', ',', ';', ':', '?' };