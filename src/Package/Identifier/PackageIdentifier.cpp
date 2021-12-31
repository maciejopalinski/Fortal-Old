#include "PackageIdentifier.h"

bool PackageIdentifier::getWildcard()
{
    return wildcard;
}

void PackageIdentifier::setWildcard(bool wildcard)
{
    this->wildcard = wildcard;
}

string PackageIdentifier::getQualifiedIdentifier()
{
    string wildcard = this->wildcard ? ".*" : "";
    return QualifiedIdentifier::getQualifiedIdentifier() + wildcard;
}