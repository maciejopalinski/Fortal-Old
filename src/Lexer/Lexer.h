#pragma once
#include "../Token/Token.h"

class LexerState
{
    public:
        char *text;
        size_t text_length = 0;

        size_t position = 0;
        size_t line = 1;
        size_t column = 1;
        char current_char = 0;
};

class LexerStateManager : public LexerState
{
    public:
        LexerState saved_state;

        void saveState();
        void loadState();
        void deleteState();
};

class Lexer : public LexerStateManager
{
    public:
        Lexer();
        ~Lexer();

        void loadFromFile(const char *filename);
        void loadFromString(const char *text, size_t size);

        Token *getNextToken(Token *token);

        void nextChar();
        size_t skipWhitespace();

        Token *getComment(Token *token);
        Token *getLineComment(Token *token);
        Token *getBlockComment(Token *token);

        Token *getKeyword(Token *token);

        Token *getSeparator(Token *token);

        Token *getOperator(Token *token);

        Token *getLiteral(Token *token);
        Token *getNumberLiteral(Token *token);
        Token *getCharLiteral(Token *token);
        Token *getStringLiteral(Token *token);
        Token *getBoolLiteral(Token *token);
        Token *getNullptrLiteral(Token *token);

        Token *getIdentifier(Token *token);

        size_t countUntil(const char *until);
        size_t readUntil(char *buffer, const char *until);
        bool eat(const char *text);
        bool eat(const char ch);
        bool match(const char *text, bool whitespace_after = false);
        bool match(const char ch, bool whitespace_after = false);

        bool isDigit(char input = 0);
        bool isAlphanumeric(char input = 0);
        bool isAlpha(char input = 0);
        bool isWhitespace(char input = 0);
        bool isNewline(char input = 0);
        bool isSeparator(char input = 0);
        bool isNULL(char input);
};