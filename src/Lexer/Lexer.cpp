#include "Lexer.h"

void LexerStateManager::saveState()
{
    saved_state.position = position;
    saved_state.line = line;
    saved_state.column = column;
    saved_state.current_char = current_char;
}

void LexerStateManager::loadState()
{
    position = saved_state.position;
    line = saved_state.line;
    column = saved_state.column;
    current_char = text[position];

    deleteState();
}

void LexerStateManager::deleteState()
{
    saved_state.position = 0;
    saved_state.line = 1;
    saved_state.column = 1;
    saved_state.current_char = 0;
}

Lexer::Lexer() {}

Lexer::~Lexer()
{
    delete[] text;
}

void Lexer::loadFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) return;

    // get file size
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // read whole file into buffer
    char *content = new char[fsize + 1];
    fread(content, 1, fsize, file);
    fclose(file);
    content[fsize] = 0;

    loadFromString(content, fsize);
    delete[] content;
}

void Lexer::loadFromString(const char *text, size_t size)
{
    this->text = new char[size + 1];
    strcpy(this->text, text);

    this->text_length = strlen(this->text);

    position = 0;
    current_char = this->text[position];
}

Token *Lexer::getNextToken(Token *token)
{
    while (!isNULL(current_char))
    {
        if (isWhitespace())
        {
            skipWhitespace();
            continue;
        }

        getComment(token);    if (*token) break;
        getKeyword(token);    if (*token) break;
        getSeparator(token);  if (*token) break;
        getOperator(token);   if (*token) break;
        getLiteral(token);    if (*token) break;
        getIdentifier(token); if (*token) break;

        printf("[ERROR] Invalid token '%c' on position %li (Ln %li, Col %li)\n", current_char, position, line, column);
        break;
    }
    return token;
}

void Lexer::nextChar()
{
    position++;
    if (position >= text_length)
    {
        current_char = 0;
        line = 0;
        column = 0;
    }
    else
    {
        if (isNewline())
        {
            line++;
            column = 1;
        }
        else column++;

        current_char = text[position];
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
                 getLineComment(token);
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
            eat(keyword);
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
            eat(separator);
            break;
        }
    }
    return token;
}

Token *Lexer::getOperator(Token *token)
{
    for (int i = 0; operators[i]; i++)
    {
        auto _operator = operators[i];
        if (match(_operator, true))
        {
            token->type = TokenType::TOKEN_OPERATOR;
            token->_operator = new TokenOperator((TokenOperatorType) i);
            eat(_operator);
            break;
        }
    }
    return token;
}

Token *Lexer::getLiteral(Token *token)
{
                 getNumberLiteral(token);
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
        char *buffer = new char[256]; // !!! size !!!
        size_t bytes_read = 0;
        while (isDigit() || match("."))
        {
            buffer[bytes_read++] = current_char;
            nextChar();
        }
        buffer[bytes_read] = 0;

        token->type = TokenType::TOKEN_LITERAL;
        if (string_contains(buffer, '.'))
        {
            auto result = atof(buffer);
            token->literal = new TokenLiteral(result);
        }
        else
        {
            auto result = atoll(buffer);
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
        char buffer = 0;
        if (isAlphanumeric())
        {
            buffer = current_char;
            nextChar();
        }
        if (!eat('\'')) return token;

        token->type = TokenType::TOKEN_LITERAL;
        token->literal = new TokenLiteral(buffer);
    }
    return token;
}

Token *Lexer::getStringLiteral(Token *token)
{
    if (match('\"') && eat('\"'))
    {
        char *buffer = new char[1024]; // !!! size !!!
        size_t bytes_read = 0;
        while (isAlphanumeric())
        {
            buffer[bytes_read++] = current_char;
            nextChar();
        }
        buffer[bytes_read] = 0;
        if (!eat('\"')) return token;

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
        char *buffer = new char[1024]; // !!! size !!!
        size_t bytes_read = 0;
        while (isAlphanumeric())
        {
            buffer[bytes_read++] = current_char;
            nextChar();
        }
        buffer[bytes_read] = 0;

        token->type = TokenType::TOKEN_IDENTIFIER;
        token->identifier = new TokenIdentifier(buffer, bytes_read);

        delete[] buffer;
    }
    return token;
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
    eat(until);

    loadState();
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
    eat(until);
    buffer[bytes_read] = 0;

    return bytes_read;
}

bool Lexer::eat(const char *text)
{
    if (match(text))
    {
        for (size_t i = 0; i < strlen(text); i++)
        {
            nextChar();
        }
        return true;
    }
    else return false;
}

bool Lexer::eat(const char ch)
{
    char input[] = { ch };
    return eat(input);
}

bool Lexer::match(const char *text, bool whitespace_after)
{
    size_t size = strlen(text) + whitespace_after;

    char *compare1 = new char[size + 1];
    strncpy(compare1, text, size);
    if (whitespace_after) compare1[size - 1] = ' ';
    compare1[size] = 0;

    char *compare2 = new char[size + 1];
    strncpy(compare2, this->text + position, size);
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