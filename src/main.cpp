#include <stdio.h>
using namespace std;

#include "Token/Token.h"
#include "Lexer/Lexer.h"
#include "Utils/Utils.h"

int main()
{
    Lexer lexer = Lexer();
    lexer.loadFromFile("test/test.c");

    while (true)
    {
        Token token = Token();

        lexer.getNextToken(&token);
        if (!token) break;

        token.debug("[DEBUG] ");
    }

    return 0;
}