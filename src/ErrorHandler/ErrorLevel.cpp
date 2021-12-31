#include "ErrorLevel.h"

ErrorLevel::ErrorLevel(ReportingLevel level)
{
    setLevel(level);
}

ErrorLevel::ErrorLevel(int level)
{
    setLevel(level);
}

void ErrorLevel::setLevel(int level)
{
    this->level = level;
}

void ErrorLevel::setFlag(int flag)
{
    this->level |= flag;
}

void ErrorLevel::unsetFlag(int flag)
{
    this->level &= ~flag;
}

void ErrorLevel::toggleFlag(int flag)
{
    this->level ^= flag;
}

bool ErrorLevel::testFlag(int flag)
{
    return (this->level & flag) == flag;
}

ErrorLevel::operator ReportingLevel() const
{
    return (ReportingLevel) this->level;
}