#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
using namespace std;

class Location
{
    public:
        char *filename;
        size_t filename_size = 0;
        size_t line = 1;
        size_t column = 1;
        size_t position = 0;

        Location();
        ~Location();

        Location *init(Location *location);
        Location *init(const char *filename, size_t line = 1, size_t column = 1, size_t position = 0);
        Location *init(const char *filename, size_t filename_size, size_t line = 1, size_t column = 1, size_t position = 0);

        void clear();
        int write(char *buffer);
};