#pragma once
#include <string>
#include <vector>
#include <memory>
using std::string;
using std::vector;
using std::shared_ptr;

#include "../../Token/TokenIdentifier.h"

class QualifiedIdentifier
{
    private:
        vector<shared_ptr<TokenIdentifier>> identifiers;

    public:
        virtual ~QualifiedIdentifier();

        void addIdentifier(shared_ptr<TokenIdentifier> token);
        vector<shared_ptr<TokenIdentifier>> getIdentifiers();
        virtual string getQualifiedIdentifier();

        virtual string getDebug(string indent, string name = "", bool last = true);
};