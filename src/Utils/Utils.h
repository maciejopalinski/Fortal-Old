#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
using namespace std;

#include "Colors.h"

extern const char *escape_characters[];
void fill_escape_characters_table();

char *replace_escape_characters(const char *input, size_t size, char *buffer);

bool string_contains(const char *string, char test);
bool string_contains(const char *string, const char *test);