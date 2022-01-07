#pragma once
#include <string>
#include <memory>
using std::string;
using std::shared_ptr;
using std::make_shared;

#include "../ErrorHandler/ErrorHandler.h"
#include "../Token/Tokens.all.h"

class Lexer
{
    private:
        ErrorHandler &error_handler;
        Location location;
        Location saved_location;

        string text;
        char current_char = 0;

    public:
        Lexer(ErrorHandler &error_handler);

        string getText();
        string getFilename();

        void loadFromFile(const char *filename);

        void saveState();
        void loadState();
        Location getLocation();

        void nextChar(size_t amount = 1);

        shared_ptr<Token> getNextToken();

        shared_ptr<TokenComment> getComment();
        shared_ptr<TokenKeyword> getKeyword();
        shared_ptr<TokenIdentifier> getIdentifier();
        shared_ptr<TokenSeparator> getSeparator();
        shared_ptr<TokenOperator> getOperator();
        shared_ptr<TokenLiteral> getLiteral();

        bool eat(const string &expected, bool report_error = false);
        bool expect(string compare1, bool whitespace_after = false);

        bool isDigit();
        bool isDigit(char c);

        bool isAlphanumeric();
        bool isAlphanumeric(char c);

        bool isAlpha();
        bool isAlpha(char c);

        bool isWhitespace();
        bool isWhitespace(char c);

        bool isNewline();
        bool isNewline(char c);

        bool isSeparator();
        bool isSeparator(char c);

        bool isNULL();
        bool isNULL(char c);
};