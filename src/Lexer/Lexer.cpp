#include "Lexer.h"
#include "../ErrorHandler/ErrorHandler.h"
#include "../Utils/Utils.h"

Lexer::Lexer()
{
    location = new Location();
    saved_location = new Location();
}

Lexer::~Lexer()
{
    delete[] text;
    delete location;
    delete saved_location;
}

void Lexer::saveState()
{
    saved_location->init(location);
}

void Lexer::loadState()
{
    location->init(saved_location);
    current_char = text[location->position];
    saved_location->clear();
}

void Lexer::loadFromFile(const char *filename)
{
    if (!filename || strlen(filename) < 1)
    {
        error_handler.no_input_files();
    }

    location->init(filename);

    FILE *file = fopen(location->filename, "r");
    if (!file)
    {
        error_handler.no_such_file_or_directory(filename);
        error_handler.no_input_files();
    };

    // get file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // read whole file into buffer
    text = new char[file_size + 1];
    size_t bytes_read = fread(text, 1, file_size, file);
    fclose(file);
    text[bytes_read] = 0;
    text_length = bytes_read;

    if (bytes_read != file_size) error_handler.file_read_error(filename);

    current_char = text[location->position];
}

Token *Lexer::getNextToken()
{
    Token *token = new Token();
    while (!isNULL(current_char))
    {
        if (isWhitespace())
        {
            skipWhitespace();
            continue;
        }

        token->location->init(location);

        getComment(token);    if (*token) break;
        getLiteral(token);    if (*token) break;
        getSeparator(token);  if (*token) break;
        getOperator(token);   if (*token) break;
        getKeyword(token);    if (*token) break;
        getIdentifier(token); if (*token) break;

        error_handler.invalid_token(location, current_char);
        break;
    }
    return token;
}

void Lexer::nextChar()
{
    location->position++;
    if (location->position >= text_length)
    {
        current_char = 0;
        location->line = 0;
        location->column = 0;
    }
    else
    {
        if (isNewline())
        {
            location->line++;
            location->column = 1;
        }
        else location->column++;

        current_char = text[location->position];
    }
}

size_t Lexer::skipWhitespace()
{
    size_t skipped = 0;
    while (isWhitespace() && !isNULL(current_char))
    {
        nextChar();
        skipped++;
    }
    return skipped;
}

Token *Lexer::getComment(Token *token)
{
    if (!*token) getLineComment(token);
    if (!*token) getBlockComment(token);
    return token;
}

Token *Lexer::getLineComment(Token *token)
{
    if (match("//") && eat("//"))
    {
        token->type = TokenType::TOKEN_COMMENT;
        token->comment = new TokenComment(TokenCommentType::TOKEN_COMMENT_LINE);

        size_t capacity = countUntil("\n");
        char *buffer = new char[capacity + 1];
        readUntil(buffer, "\n");
        eat("\n");

        token->comment->content = buffer;
    }
    return token;
}

Token *Lexer::getBlockComment(Token *token)
{
    if (match("/*") && eat("/*"))
    {
        token->type = TokenType::TOKEN_COMMENT;
        token->comment = new TokenComment(TokenCommentType::TOKEN_COMMENT_BLOCK);

        size_t capacity = countUntil("*/");
        char *buffer = new char[capacity + 1];
        readUntil(buffer, "*/");
        eat("*/");

        token->comment->content = buffer;
    }
    return token;
}

Token *Lexer::getKeyword(Token *token)
{
    for (int i = 0; keywords[i]; i++)
    {
        auto keyword = keywords[i];
        if (match(keyword, true))
        {
            token->type = TokenType::TOKEN_KEYWORD;
            token->keyword = new TokenKeyword((TokenKeywordType) i);
            eat(keyword, true);
            break;
        }
    }
    return token;
}

Token *Lexer::getSeparator(Token *token)
{
    for (int i = 0; separators[i]; i++)
    {
        auto separator = separators[i];
        if (match(separator))
        {
            token->type = TokenType::TOKEN_SEPARATOR;
            token->separator = new TokenSeparator((TokenSeparatorType) i);
            eat(separator, true);
            break;
        }
    }
    return token;
}

Token *Lexer::getOperator(Token *token)
{
    const char *max_match_str = nullptr;
    int max_match_enum;
    size_t max_match_size = 0;

    for (int i = 0; operators[i]; i++)
    {
        auto _operator = operators[i];
        size_t operator_size = strlen(_operator);
        if (match(_operator))
        {
            if (operator_size >= max_match_size)
            {
                max_match_str = _operator;
                max_match_enum = i;
                max_match_size = operator_size;
            }
        }
    }

    if (max_match_str)
    {
        token->type = TokenType::TOKEN_OPERATOR;
        token->_operator = new TokenOperator((TokenOperatorType) max_match_enum);
        eat(max_match_str, true);
    }
    return token;
}

Token *Lexer::getLiteral(Token *token)
{
    if (!*token) getNumberLiteral(token);
    if (!*token) getCharLiteral(token);
    if (!*token) getStringLiteral(token);
    if (!*token) getBoolLiteral(token);
    if (!*token) getNullptrLiteral(token);
    return token;
}

Token *Lexer::getNumberLiteral(Token *token)
{
    if (isDigit())
    {
        auto condition = [] (char c) { return isdigit(c) || c == '.' || c == '-' || c == 'e'; };
        size_t capacity = countUntil(condition);
        char *buffer = new char[capacity + 1];
        readUntil(buffer, condition);

        token->type = TokenType::TOKEN_LITERAL;
        if (string_contains(buffer, '.') || string_contains(buffer, 'e'))
        {
            double result = atof(buffer);
            token->literal = new TokenLiteral(result);
        }
        else
        {
            long long result = atoll(buffer);
            token->literal = new TokenLiteral(result);
        }

        delete[] buffer;
    }
    return token;
}

Token *Lexer::getCharLiteral(Token *token)
{
    if (match('\'') && eat('\''))
    {
        auto condition = [] (char c) { return (bool) isalnum(c); };
        size_t capacity = countUntil(condition);
        char *buffer = new char[capacity + 1];
        size_t bytes_read = readUntil(buffer, condition);
        eat('\'', true);

        token->type = TokenType::TOKEN_LITERAL;
        if (bytes_read <= 1) token->literal = new TokenLiteral(buffer[0]);
        else
        {
            token->literal = new TokenLiteral(buffer, bytes_read);
            error_handler.assume_multichar_is_string(token);
        }

        delete[] buffer;
    }
    return token;
}

Token *Lexer::getStringLiteral(Token *token)
{
    if (match('\"') && eat('\"'))
    {
        auto condition = [] (char c) { return (bool) isalnum(c); };
        size_t capacity = countUntil(condition);
        char *buffer = new char[capacity + 1];
        size_t bytes_read = readUntil(buffer, condition);
        eat('\"', true);

        token->type = TokenType::TOKEN_LITERAL;
        token->literal = new TokenLiteral(buffer, bytes_read);

        delete[] buffer;
    }
    return token;
}

Token *Lexer::getBoolLiteral(Token *token)
{
    if (match("true", true) && eat("true"))
    {
        token->type = TokenType::TOKEN_LITERAL;
        token->literal = new TokenLiteral(true);
    }
    if (match("false", true) && eat("false"))
    {
        token->type = TokenType::TOKEN_LITERAL;
        token->literal = new TokenLiteral(false);
    }
    return token;
}

Token *Lexer::getNullptrLiteral(Token *token)
{
    if (match("nullptr", true) && eat("nullptr"))
    {
        token->type = TokenType::TOKEN_LITERAL;
        token->literal = new TokenLiteral((void*) true);
    }
    return token;
}

Token *Lexer::getIdentifier(Token *token)
{
    if (isAlpha())
    {
        auto condition = [] (char c) { return (bool) isalnum(c); };
        size_t capacity = countUntil(condition);
        char *buffer = new char[capacity + 1];
        size_t bytes_read = readUntil(buffer, condition);

        token->type = TokenType::TOKEN_IDENTIFIER;
        token->identifier = new TokenIdentifier(buffer, bytes_read);

        delete[] buffer;
    }
    return token;
}

size_t Lexer::countUntil(bool (*condition)(char))
{
    size_t bytes_read = 0;
    saveState();

    while (condition(current_char) && !isNULL(current_char))
    {
        nextChar();
        bytes_read++;
    }

    loadState();
    return bytes_read;
}

size_t Lexer::countUntil(const char *until)
{
    size_t bytes_read = 0;
    saveState();

    while (!match(until) && !isNULL(current_char))
    {
        nextChar();
        bytes_read++;
    }

    loadState();
    return bytes_read;
}

size_t Lexer::readUntil(char *buffer, bool (*condition)(char))
{
    size_t bytes_read = 0;

    while (condition(current_char) && !isNULL(current_char))
    {
        buffer[bytes_read++] = current_char;
        nextChar();
    }
    buffer[bytes_read] = 0;

    return bytes_read;
}

size_t Lexer::readUntil(char *buffer, const char *until)
{
    size_t bytes_read = 0;

    while (!match(until) && !isNULL(current_char))
    {
        buffer[bytes_read++] = current_char;
        nextChar();
    }
    buffer[bytes_read] = 0;

    return bytes_read;
}

size_t Lexer::readFromUntil(char *buffer, size_t start_from, const char *until)
{
    saveState();
    location->position = start_from;
    current_char = text[location->position];

    size_t bytes_read = readUntil(buffer, until);

    loadState();
    return bytes_read;
}

int Lexer::eat(const char *expected, bool report_error)
{
    size_t expected_size = strlen(expected);

    if (match(expected))
    {
        for (size_t i = 0; i < expected_size; i++)
        {
            nextChar();
        }
        return 1;
    }
    else if (report_error)
    {
        char *unexpected = new char[expected_size + 1];
        strncpy(unexpected, text + location->position, expected_size);
        unexpected[expected_size] = 0;

        error_handler.unexpected_token(location, expected, unexpected);
        delete[] unexpected;

        return -1;
    }
    else return 0;
}

int Lexer::eat(const char ch, bool report_error)
{
    char input[] = { ch };
    return eat(input, report_error);
}

bool Lexer::match(const char *text, bool whitespace_after)
{
    size_t size = strlen(text) + whitespace_after;

    char *compare1 = new char[size + 1];
    strncpy(compare1, text, size);
    if (whitespace_after) compare1[size - 1] = ' ';
    compare1[size] = 0;

    char *compare2 = new char[size + 1];
    strncpy(compare2, this->text + location->position, size);
    compare2[size] = 0;

    bool compare_result = !strncmp(compare1, compare2, size - whitespace_after);
    if (whitespace_after)
    {
        char c = compare2[size - 1];
        compare_result &= (isWhitespace(c) || isSeparator(c) || isNewline(c) || isNULL(c));
    }

    delete[] compare1;
    delete[] compare2;
    return compare_result;
}

bool Lexer::match(const char ch, bool whitespace_after)
{
    char input[] = { ch };
    return match(input, whitespace_after);
}

bool Lexer::isDigit(char input)
{
    if (input) return isdigit(input);
    else return isDigit(current_char);
}

bool Lexer::isAlphanumeric(char input)
{
    if (input) return isalnum(input);
    if (!current_char) return false;
    else return isAlphanumeric(current_char);
}

bool Lexer::isAlpha(char input)
{
    if (input) return isalpha(input);
    if (!current_char) return false;
    else return isAlpha(current_char);
}

bool Lexer::isWhitespace(char input)
{
    if (input) return isspace(input);
    if (!current_char) return false;
    else return isWhitespace(current_char);
}

bool Lexer::isNewline(char input)
{
    if (input) return input == '\n';
    if (!current_char) return false;
    else return isNewline(current_char);
}

bool Lexer::isSeparator(char input)
{
    if (input)
    {
        bool found = false;
        for (int i = 0; separators[i]; i++)
        {
            if ((found = (input == separators[i]))) break;
        }
        return found;
    }
    if (!current_char) return false;
    else return isSeparator(current_char);
}

bool Lexer::isNULL(char input)
{
    return input == 0;
}