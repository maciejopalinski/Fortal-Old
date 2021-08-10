#include "Token.h"
#include "../ErrorHandler/ErrorHandler.h"

Token::Token(TokenType type) : type(type)
{
    location = new Location();
}

Token::~Token()
{
    if (type == TOKEN_COMMENT)    delete comment;
    if (type == TOKEN_KEYWORD)    delete keyword;
    if (type == TOKEN_SEPARATOR)  delete separator;
    if (type == TOKEN_OPERATOR)   delete _operator;
    if (type == TOKEN_LITERAL)    delete literal;
    if (type == TOKEN_IDENTIFIER) delete identifier;

    delete location;
}

void Token::debug()
{
    if (type == TOKEN_COMMENT)    comment   ->debug(location);
    if (type == TOKEN_KEYWORD)    keyword   ->debug(location);
    if (type == TOKEN_SEPARATOR)  separator ->debug(location);
    if (type == TOKEN_OPERATOR)   _operator ->debug(location);
    if (type == TOKEN_LITERAL)    literal   ->debug(location);
    if (type == TOKEN_IDENTIFIER) identifier->debug(location);
}

Token::operator bool() const
{
    return type != TOKEN_NULL;
}

Token::operator TokenType() const
{
    return type;
}