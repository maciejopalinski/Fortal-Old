#pragma once
#include "../Token/Token.h"

class Lexer
{
    public:
        Lexer();
        ~Lexer();

        char *text;
        size_t text_length = 0;

        Location location;
        Location saved_location;
        char current_char = 0;

        void saveState();
        void loadState();

        void loadFromFile(const char *filename);
        void setTokenPosition(Token *token);
        char *describeLocation(char *buffer);

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

        size_t countUntil(bool (*condition)(char));
        size_t countUntil(const char *until);
        size_t readUntil(char *buffer, bool (*condition)(char));
        size_t readUntil(char *buffer, const char *until);
        size_t readFromUntil(char *buffer, size_t start_from, const char *until);
        int eat(const char *text, bool report_error = false);
        int eat(const char ch, bool report_error = false);
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

extern Lexer lexer;