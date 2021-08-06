#include "Utils.h"

const char *escape_characters[64] = { NULL };

void fill_escape_characters_table()
{
    escape_characters['\0'] = COLOR_LIGHT_BLACK "\\0" COLOR_RESET;
    escape_characters['\a'] = COLOR_LIGHT_BLACK "\\a" COLOR_RESET;
    escape_characters['\b'] = COLOR_LIGHT_BLACK "\\b" COLOR_RESET;
    escape_characters['\f'] = COLOR_LIGHT_BLACK "\\f" COLOR_RESET;
    escape_characters['\n'] = COLOR_LIGHT_BLACK "\\n" COLOR_RESET;
    escape_characters['\r'] = COLOR_LIGHT_BLACK "\\r" COLOR_RESET;
    escape_characters['\t'] = COLOR_LIGHT_BLACK "\\t" COLOR_RESET;
    escape_characters['\v'] = COLOR_LIGHT_BLACK "\\v" COLOR_RESET;
}

char *replace_escape_characters(const char *input, size_t size, char *buffer)
{
    fill_escape_characters_table();

    size_t i, j;
    for (i = 0, j = 0; i < size; i++)
    {
        auto ch = input[i];
        auto esc = escape_characters[(int) ch];

        if (esc == NULL) buffer[j++] = ch;
        else
        {
            buffer[j] = 0;
            strcat(buffer, esc);
            j += strlen(esc);
        }
    }
    buffer[j] = 0;
    return buffer;
}

bool string_contains(const char *string, char test)
{
    return strchr(string, test);
}

bool string_contains(const char *string, const char *test)
{
    return strstr(string, test);
}