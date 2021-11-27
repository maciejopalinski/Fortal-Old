#pragma once
#include "TokenBase.hpp"

template<class T>
class TokenHasType : public TokenBase
{
    private:
    T type;

    protected:
    static vector<string> lexer_matches;

    public:
    TokenHasType(T type) { this->type = type; }

    T getType() { return this->type; }

    void setType(T type) { this->type = type; }

    virtual string getTypeString(T type) = 0;

    static vector<string> getLexerMatches();
    static T getLexerTypeFromMatch(string match);
    static bool getLexerWhitespaceAfter();
};
