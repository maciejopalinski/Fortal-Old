#pragma once
#include "../Utils/Format.hpp"

class Location
{
    public:
        string filename = "";

        size_t position = 0;
        size_t line = 1;
        size_t column = 1;

        string getPrefix()
        {
            return format("%s:%li:%li", filename.c_str(), line, column);
        }

        void reset()
        {
            position = 0;
            line = 1;
            column = 1;
        }
};
