#pragma once
#include <string.h>
#include "../Token/TokenBase.hpp"
#include "../Token/TokenComment.hpp"
#include "../Token/TokenKeyword.hpp"
#include "../Token/TokenIdentifier.hpp"
#include "../Token/TokenSeparator.hpp"
#include "../Token/TokenBracket.hpp"
#include "../Token/TokenOperator.hpp"
#include "../Token/TokenLiteral.hpp"

class Lexer
{
    private:
        ErrorHandler &error_handler;
        Location location;
        Location saved_location;

        string text;
        char current_char = 0;

    public:

        Lexer(ErrorHandler &error_handler) : error_handler(error_handler) {}

        string getText() { return this->text; }

        void loadFromFile(string filename)
        {
            if (filename.length() < 1)
            {
                error_handler.throw_no_input_files();
            }

            location.filename = filename;

            FILE *file = fopen(filename.c_str(), "r");
            if (!file)
            {
                error_handler.log_no_such_file_or_directory(filename);
                error_handler.throw_no_input_files();
            }

            // get file size
            fseek(file, 0, SEEK_END);
            size_t file_size = ftell(file);
            fseek(file, 0, SEEK_SET);

            // read whole file into buffer
            unique_ptr<char[]> buf(new char[file_size + 1]);
            size_t bytes_read = fread(buf.get(), 1, file_size, file);
            fclose(file);
            buf[bytes_read] = 0;

            text = string(buf.get(), buf.get() + bytes_read);
            current_char = text[location.position];
        }

        void saveState()
        {
            saved_location = Location(location);
        }

        void loadState()
        {
            location = Location(saved_location);
            saved_location.reset();
            current_char = text[location.position];
        }

        void nextChar(size_t amount = 1)
        {
            if (amount > 1)
            {
                for (size_t i = 0; i < amount; i++)
                {
                    nextChar();
                }
                return;
            }

            location.position++;
            if (location.position >= text.length())
            {
                current_char = 0;
                location.line = 0;
                location.column = 0;
            }
            else
            {
                if (isNewline())
                {
                    location.line++;
                    location.column = 1;
                }
                else location.column++;

                current_char = text[location.position];
            }
        }

        TokenBase *getNextToken()
        {
            TokenBase *token = nullptr;
            Location token_location;

            while (!isNULL())
            {
                if (isWhitespace())
                {
                    nextChar();
                    continue;
                }

                token_location = Location(location);

                if ((token = getComment())) break;
                if ((token = getKeyword())) break;
                if ((token = getLiteral())) break;
                if ((token = getOperator())) break;
                if ((token = getSeparator())) break;
                if ((token = getBracket())) break;
                if ((token = getIdentifier())) break;

                error_handler.throw_invalid_token(location, current_char);
                break;
            }

            if (token) token->location = token_location;
            return token;
        }

        TokenComment *getComment()
        {
            TokenComment *token = nullptr;
            if (eat("//"))
            {
                token = new TokenComment(TOKEN_COMMENT_LINE);

                size_t until = text.find("\n", location.position);
                string content = text.substr(location.position, until - location.position);
                token->setContent(content);

                if (!eat(content, true) || !eat("\n", true))
                {
                    return nullptr;
                }
            }
            else if (eat("/*"))
            {
                token = new TokenComment(TOKEN_COMMENT_BLOCK);

                size_t until = text.find("*/", location.position);
                string content = text.substr(location.position, until - location.position);
                token->setContent(content);

                if (!eat(content, true) || !eat("*/", true))
                {
                    return nullptr;
                }
            }
            return token;
        }

        TokenKeyword *getKeyword()
        {
            TokenKeyword *token = nullptr;
            auto matches = TokenKeyword::getLexerMatches();
            bool whitespace_after = TokenKeyword::getLexerWhitespaceAfter();

            for (string &match : matches)
            {
                if (expect(match, whitespace_after))
                {
                    token = new TokenKeyword(TokenKeyword::getLexerTypeFromMatch(match));
                    eat(match, true);
                    break;
                }
            }
            return token;
        }

        TokenIdentifier *getIdentifier()
        {
            TokenIdentifier *token = nullptr;

            if (isAlpha())
            {
                string name;
                while (isAlphanumeric())
                {
                    name += current_char;
                    nextChar();
                }

                token = new TokenIdentifier(name);
            }

            return token;
        }

        TokenSeparator *getSeparator()
        {
            TokenSeparator *token = nullptr;
            if (isSeparator())
            {
                string match = " ";
                match[0] = current_char;

                token = new TokenSeparator(TokenSeparator::getLexerTypeFromMatch(current_char));
                eat(match, true);
            }
            return token;
        }

        TokenBracket *getBracket()
        {
            TokenBracket *token = nullptr;
            if (isBracket())
            {
                string match = " ";
                match[0] = current_char;

                token = new TokenBracket(TokenBracket::getLexerTypeFromMatch(current_char));
                eat(match, true);
            }
            return token;
        }

        TokenOperator *getOperator()
        {
            TokenOperator *token = nullptr;
            auto matches = TokenOperator::getLexerMatches();
            bool whitespace_after = TokenOperator::getLexerWhitespaceAfter();

            string last_match = "";
            for (string &match : matches)
            {
                if (expect(match, whitespace_after) && match.size() > last_match.size())
                {
                    last_match = match;
                }
            }

            if (last_match.size() > 0)
            {
                token = new TokenOperator(TokenOperator::getLexerTypeFromMatch(last_match));
                eat(last_match, true);
            }
            return token;
        }

        TokenLiteral *getLiteral()
        {
            TokenLiteral *token = nullptr;

            if (isDigit() || expect("-"))
            {
                string number = "";
                while (isDigit() || expect(".") || expect("e") || expect("-"))
                {
                    number += current_char;
                    nextChar();
                }

                bool has_dot = number.find(".") != number.npos;
                bool has_sci = number.find("e") != number.npos;
                if (!has_dot && !has_sci)
                {
                    token = new TokenLiteral(TOKEN_LITERAL_INT);
                    token->setValue(atoll(number.c_str()));
                }
                else
                {
                    token = new TokenLiteral(TOKEN_LITERAL_DOUBLE);
                    token->setValue(atof(number.c_str()));
                }
            }
            else if ((expect("\'") && eat("\'")) || (expect("\"") && eat("\"")))
            {
                string str = "";
                while (!expect("\'") && !expect("\""))
                {
                    str += current_char;
                    nextChar();
                }
                eat("\'");
                eat("\"");

                if (str.size() == 1)
                {
                    token = new TokenLiteral(TOKEN_LITERAL_CHAR);
                    token->setValue((long long) str[0]);
                }
                else if (str.size() > 1)
                {
                    token = new TokenLiteral(TOKEN_LITERAL_STRING);
                    token->setValue(str.c_str());
                }
                else
                {
                    error_handler.log_empty_character_literal(location);
                    token = new TokenLiteral(TOKEN_LITERAL_INT);
                    token->setValue((long long) 0);
                }
            }
            else if (expect("true", true) || expect("false", true))
            {
                token = new TokenLiteral(TOKEN_LITERAL_BOOL);
                if (eat("true")) token->setValue((long long) true);
                else if (eat("false")) token->setValue((long long) false);
                else token->setNULL(true);
            }
            else if (expect("null", true))
            {
                token = new TokenLiteral(TOKEN_LITERAL_NULL);
                token->setNULL(true);
                eat("null", true);
            }
            return token;
        }

        bool eat(string expected, bool report_error = false)
        {
            if (expect(expected))
            {
                nextChar(expected.length());
                return true;
            }
            else if (report_error)
            {
                string unexpected = text.substr(location.position, expected.size());

                error_handler.log_unexpected_token(
                    location,
                    expected,
                    unexpected
                );
            }
            return false;
        }

        bool expect(string compare1, bool whitespace_after = false)
        {
            size_t size = compare1.length() + whitespace_after;

            if (whitespace_after) compare1 += ' ';

            string compare2 = text.substr(location.position, size);

            bool result = !strncmp(compare1.c_str(), compare2.c_str(), size - whitespace_after);
            if (whitespace_after)
            {
                char c = compare2[compare2.size() - 1];
                if (compare2.size() < compare1.size()) c = 0;

                result &= (isWhitespace(c) || isSeparator(c) || isBracket(c) || isNewline(c) || isNULL(c));
            }
            return result;
        }

        bool isDigit() { return isDigit(current_char); }
        bool isDigit(char c) { return isdigit(c); }

        bool isAlphanumeric() { return isAlphanumeric(current_char); }
        bool isAlphanumeric(char c) { return isalnum(c) || c == '_'; }

        bool isAlpha() { return isAlpha(current_char); }
        bool isAlpha(char c) { return isalpha(c) || c == '_'; }

        bool isWhitespace() { return isWhitespace(current_char); }
        bool isWhitespace(char c) { return isspace(c); }

        bool isNewline() { return isNewline(current_char); }
        bool isNewline(char c) { return c == '\n'; }

        bool isSeparator() { return isSeparator(current_char); }
        bool isSeparator(char c)
        {
            auto matches = TokenSeparator::getLexerMatches();
            for (char cc : matches)
            {
                if (c == cc) return true;
            }
            return false;
        }

        bool isBracket() { return isBracket(current_char); }
        bool isBracket(char c)
        {
            auto matches = TokenBracket::getLexerMatches();
            for (char cc : matches)
            {
                if (c == cc) return true;
            }
            return false;
        }

        bool isNULL() { return isNULL(current_char); }
        bool isNULL(char c) { return c == 0; }
};