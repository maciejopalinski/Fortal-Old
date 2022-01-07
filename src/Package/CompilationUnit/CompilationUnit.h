#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

#include "Definition/Definition.h"
#include "../Identifier/PackageIdentifier.h"
#include "../../Token/TokenIdentifier.h"

class CompilationUnit
{
    private:
        string filename;

        shared_ptr<PackageIdentifier> package;

        vector<shared_ptr<PackageIdentifier>> imports;
        vector<shared_ptr<Definition>> definitions;

    public:
        CompilationUnit(shared_ptr<PackageIdentifier> package, const string &filename);

        string getFilename();
        shared_ptr<PackageIdentifier> getPackageIdentifier();

        void addImport(shared_ptr<PackageIdentifier> import);
        vector<shared_ptr<PackageIdentifier>> getImports();

        shared_ptr<Definition> addDefinition(shared_ptr<Definition> definition);
        vector<shared_ptr<Definition>> getDefinitions();

        shared_ptr<Definition> getDefinitionByIdentifier(shared_ptr<TokenIdentifier> identifier);

        string getDebug(string indent, bool last = true);
};