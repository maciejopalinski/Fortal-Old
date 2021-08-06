#include "TokenIdentifier.h"

TokenIdentifier::TokenIdentifier(char *name, size_t size)
{
    this->name = new char[size + 1];
    strcpy(this->name, name);
}

TokenIdentifier::~TokenIdentifier()
{
    // printf("[DEBUG] TokenIdentifier destroyed\n");
    delete[] name;
}

void TokenIdentifier::debug(const char *prefix)
{
    printf
    (
        "%sTokenIdentifier(name='%s')\n",
        prefix,
        name
    );
}