#include "Lexer.h"

Lexer::Lexer(ErrorHandler &error_handler) : error_handler(error_handler) {}

string Lexer::getText()
{
    return this->text;
}

string Lexer::getFilename()
{
    return this->location.filename;
}

void Lexer::loadFromFile(const char *filename)
{
    if (strlen(filename) < 1)
    {
        error_handler.throw_no_input_files();
    }

    location.filename = filename;

    FILE *file = fopen(filename, "r");
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
    text += "\n";

    current_char = text[location.position];
}

void Lexer::saveState()
{
    saved_location = Location(location);
}

void Lexer::loadState()
{
    location = Location(saved_location);
    saved_location.reset();
    current_char = text[location.position];
}

Location Lexer::getLocation()
{
    return location;
}

void Lexer::nextChar(size_t amount)
{
    if (amount > 1)
    {
        for (size_t i = 0; i < amount; i++)
        {
            nextChar();
        }
        return;
    }
    else if (amount == 1)
    {
        location.position++;

        if (isNewline())
        {
            location.line++;
            location.column = 1;
        }
        else location.column++;

        if (location.position >= text.length())
        {
            current_char = 0;
        }
        else
        {
            current_char = text[location.position];
        }
    }
}

shared_ptr<Token> Lexer::getNextToken()
{
    shared_ptr<Token> token;
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
        if ((token = getIdentifier())) break;

        error_handler.throw_invalid_token(location, current_char);
        break;
    }

    if (token) token->location = token_location;
    return token;
}

shared_ptr<TokenComment> Lexer::getComment()
{
    shared_ptr<TokenComment> token;
    if (eat("//"))
    {
        token = make_shared<TokenComment>(TokenComment(TOKEN_COMMENT_LINE));

        size_t until   = text.find("\n", location.position);
        string content = text.substr(location.position, until - location.position);
        token->setContent(content);

        if (!(eat(content, true) && eat("\n", true)))
        {
            return nullptr;
        }
    }
    else if (eat("/*"))
    {
        token = make_shared<TokenComment>(TokenComment(TOKEN_COMMENT_BLOCK));

        size_t until   = text.find("*/", location.position);
        string content = text.substr(location.position, until - location.position);
        token->setContent(content);

        if (!eat(content, true) || !eat("*/", true))
        {
            return nullptr;
        }
    }
    return token;
}

shared_ptr<TokenKeyword> Lexer::getKeyword()
{
    shared_ptr<TokenKeyword> token   = nullptr;
    auto matches          = TokenKeyword::lexer_matches;
    bool whitespace_after = TokenKeyword::lexer_whitespace_after;

    for (string match : matches)
    {
        if (expect(match, whitespace_after))
        {
            token = make_shared<TokenKeyword>(TokenKeyword(TokenKeyword::getLexerTypeFromMatch(match)));
            eat(match, true);
            break;
        }
    }
    return token;
}

shared_ptr<TokenIdentifier> Lexer::getIdentifier()
{
    shared_ptr<TokenIdentifier> token = nullptr;

    if (isAlpha())
    {
        string name;
        while (isAlphanumeric())
        {
            name += current_char;
            nextChar();
        }

        token = make_shared<TokenIdentifier>(TokenIdentifier(name));
    }

    return token;
}

shared_ptr<TokenSeparator> Lexer::getSeparator()
{
    shared_ptr<TokenSeparator> token = nullptr;
    if (isSeparator())
    {
        string match = " ";
        match[0]     = current_char;

        token = make_shared<TokenSeparator>(TokenSeparator(TokenSeparator::getLexerTypeFromMatch(current_char)));
        eat(match, true);
    }
    return token;
}

shared_ptr<TokenOperator> Lexer::getOperator()
{
    shared_ptr<TokenOperator> token  = nullptr;
    auto matches          = TokenOperator::lexer_matches;
    bool whitespace_after = TokenOperator::lexer_whitespace_after;

    string last_match = "";
    for (string match : matches)
    {
        if (expect(match, whitespace_after) && match.size() > last_match.size())
        {
            last_match = match;
        }
    }

    if (last_match.size() > 0)
    {
        token = make_shared<TokenOperator>(TokenOperator(TokenOperator::getLexerTypeFromMatch(last_match)));
        eat(last_match, true);
    }
    return token;
}

shared_ptr<TokenLiteral> Lexer::getLiteral()
{
    shared_ptr<TokenLiteral> token = nullptr;

    if (isDigit())
    {
        string number = "";
        while (isDigit())
        {
            number += current_char;
            nextChar();
        }

        if (eat(".")) number += ".";

        while (isDigit())
        {
            number += current_char;
            nextChar();
        }

        if (eat("e"))
        {
            number += "e";
            if (eat("-")) number += "-";

            while (isDigit())
            {
                number += current_char;
                nextChar();
            }
        }

        bool has_dot = number.find(".") != number.npos;
        bool has_sci = number.find("e") != number.npos;
        if (!has_dot && !has_sci)
        {
            token = make_shared<TokenLiteral>(TokenLiteral(TOKEN_LITERAL_INT));
            token->setValue(atoll(number.c_str()));
        }
        else
        {
            token = make_shared<TokenLiteral>(TokenLiteral(TOKEN_LITERAL_DOUBLE));
            token->setValue(atof(number.c_str()));
        }
    }
    else if ((expect("\'") && eat("\'")) || (expect("\"") && eat("\"")))
    {
        string str = "";
        while (!expect("\'") && !expect("\"") && current_char)
        {
            str += current_char;
            nextChar();
        }
        eat("\'");
        eat("\"");

        if (str.size() == 1)
        {
            token = make_shared<TokenLiteral>(TokenLiteral(TOKEN_LITERAL_CHAR));
            token->setValue((long long) str[0]);
        }
        else if (str.size() > 1)
        {
            token = make_shared<TokenLiteral>(TokenLiteral(TOKEN_LITERAL_STRING));
            token->setValue(str.c_str());
        }
        else
        {
            error_handler.log_empty_character_literal(location);
            token = make_shared<TokenLiteral>(TokenLiteral(TOKEN_LITERAL_INT));
            token->setValue((long long) 0);
        }
    }
    else if (expect("true", true) || expect("false", true))
    {
        token = make_shared<TokenLiteral>(TokenLiteral(TOKEN_LITERAL_BOOL));
        if (eat("true")) token->setValue((long long) true);
        else if (eat("false")) token->setValue((long long) false);
        else token->setNULL(true);
    }
    else if (expect("null", true))
    {
        token = make_shared<TokenLiteral>(TokenLiteral(TOKEN_LITERAL_NULL));
        token->setNULL(true);
        eat("null", true);
    }
    return token;
}

bool Lexer::eat(const string &expected, bool report_error)
{
    if (expect(expected))
    {
        nextChar(expected.length());
        return true;
    }
    else if (report_error)
    {
        string unexpected = text.substr(location.position, expected.size());

        error_handler.throw_unexpected_token(location, expected, unexpected);
    }
    return false;
}

bool Lexer::expect(string compare1, bool whitespace_after)
{
    size_t size = compare1.length() + whitespace_after;

    if (whitespace_after) compare1 += ' ';

    string compare2 = text.substr(location.position, size);

    bool result = !strncmp(compare1.c_str(), compare2.c_str(), size - whitespace_after);
    if (whitespace_after)
    {
        char c = compare2[compare2.size() - 1];
        if (compare2.size() < compare1.size()) c = 0;

        result &= isSeparator(c);
    }
    return result;
}

bool Lexer::isDigit()
{
    return isDigit(current_char);
}
bool Lexer::isDigit(char c)
{
    return isdigit(c);
}

bool Lexer::isAlphanumeric()
{
    return isAlphanumeric(current_char);
}
bool Lexer::isAlphanumeric(char c)
{
    return isalnum(c) || c == '_';
}

bool Lexer::isAlpha()
{
    return isAlpha(current_char);
}
bool Lexer::isAlpha(char c)
{
    return isalpha(c) || c == '_';
}

bool Lexer::isWhitespace()
{
    return isWhitespace(current_char);
}
bool Lexer::isWhitespace(char c)
{
    return isspace(c);
}

bool Lexer::isNewline()
{
    return isNewline(current_char);
}
bool Lexer::isNewline(char c)
{
    return c == '\n';
}

bool Lexer::isSeparator()
{
    return isSeparator(current_char);
}
bool Lexer::isSeparator(char c)
{
    bool matchesSeparator = TokenSeparator::getLexerTypeFromMatch(c) != -1;
    return matchesSeparator || isWhitespace(c) || isNewline(c);
}

bool Lexer::isNULL()
{
    return isNULL(current_char);
}
bool Lexer::isNULL(char c)
{
    return c == 0;
}