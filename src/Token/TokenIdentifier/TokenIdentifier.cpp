#include "TokenIdentifier.h"
#include "../../ErrorHandler/ErrorHandler.h"
#include "../../Utils/Utils.h"

TokenIdentifier::TokenIdentifier(char *name, size_t size)
{
    this->name = new char[size + 1];
    strcpy(this->name, name);
}

TokenIdentifier::~TokenIdentifier()
{
    delete[] name;
}

void TokenIdentifier::debug(Location *location)
{
    error_handler.logLocation
    (
        E_DEBUG,
        location,
        "TokenIdentifier(name='%s')\n",
        name
    );
}