#pragma once

#include "QualifiedIdentifier.h"

class PackageIdentifier : public QualifiedIdentifier
{
    private:
        bool wildcard = false;

    public:
        bool getWildcard();
        void setWildcard(bool wildcard);

        string getQualifiedIdentifier();
};