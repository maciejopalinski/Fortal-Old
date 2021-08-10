#include "Location.h"

Location::Location()
{
    filename = new char[8];
}

Location::~Location()
{
    delete[] filename;
}

Location *Location::init(Location *location)
{
    return init(
        location->filename,
        location->filename_size,
        location->line,
        location->column,
        location->position
    );
}

Location *Location::init(const char *filename, size_t line, size_t column, size_t position)
{
    return init(filename, strlen(filename), line, column, position);
}

Location *Location::init(const char *filename, size_t filename_size, size_t line, size_t column, size_t position)
{
    delete[] this->filename;
    this->filename = new char[filename_size + 1];
    strcpy(this->filename, filename);

    this->filename_size = filename_size;
    this->line = line;
    this->column = column;
    this->position = position;

    return this;
}

void Location::clear()
{
    this->line = 1;
    this->column = 1;
    this->position = 0;
}

int Location::write(char *buffer)
{
    return sprintf(buffer, "%s:%li:%li", filename, line, column);
}