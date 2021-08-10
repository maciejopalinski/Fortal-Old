#include "ErrorLevel.h"

ErrorLevel::ErrorLevel(ReportingLevel level) : level(level) {}
ErrorLevel::ErrorLevel(int level) : level(level) {}

ReportingLevel ErrorLevel::setLevel(int level)
{
    this->level = level;
    return (ReportingLevel) this->level;
}

ReportingLevel ErrorLevel::setFlag(int flag)
{
    this->level |= flag;
    return (ReportingLevel) this->level;
}

ReportingLevel ErrorLevel::unsetFlag(int flag)
{
    this->level &= ~flag;
    return (ReportingLevel) this->level;
}

ReportingLevel ErrorLevel::toggleFlag(int flag)
{
    this->level ^= flag;
    return (ReportingLevel) this->level;
}

bool ErrorLevel::testFlag(int flag)
{
    return ((this->level & flag) == flag);
}

ErrorLevel::operator ReportingLevel() const
{
    return (ReportingLevel) this->level;
}