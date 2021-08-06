#include "Token.h"

Token::Token(TokenType type) : type(type) {}

Token::~Token()
{
    // printf("[DEBUG] Token destroyed\n");

    if (type == TOKEN_COMMENT)    delete comment;
    if (type == TOKEN_KEYWORD)    delete keyword;
    if (type == TOKEN_SEPARATOR)  delete separator;
    if (type == TOKEN_OPERATOR)   delete _operator;
    if (type == TOKEN_LITERAL)    delete literal;
    if (type == TOKEN_IDENTIFIER) delete identifier;
}

void Token::debug(const char *prefix)
{
    if (type == TOKEN_COMMENT)    comment   ->debug(prefix);
    if (type == TOKEN_KEYWORD)    keyword   ->debug(prefix);
    if (type == TOKEN_SEPARATOR)  separator ->debug(prefix);
    if (type == TOKEN_OPERATOR)   _operator ->debug(prefix);
    if (type == TOKEN_LITERAL)    literal   ->debug(prefix);
    if (type == TOKEN_IDENTIFIER) identifier->debug(prefix);
}

Token::operator bool() const
{
    // printf
    // (
    //     "[DEBUG] Token has been asked for its bool value (%s != TOKEN_NULL ---> %s)\n",
    //     TokenType_enum_to_string(type),
    //     (type != TOKEN_NULL) ? "true" : "false"
    // );

    return type != TOKEN_NULL;
}

Token::operator TokenType() const
{
    // printf
    // (
    //     "[DEBUG] Token has been asked for its TokenType value (%s)\n",
    //     TokenType_enum_to_string(type)
    // );

    return type;
}