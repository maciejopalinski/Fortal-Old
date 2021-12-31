#include "CompilationUnit.h"
#include "../Package.h"

CompilationUnit::CompilationUnit(shared_ptr<PackageIdentifier> package, string filename)
{
    this->package = package;
    this->filename = filename;
}

string CompilationUnit::getFilename()
{
    return filename;
}

shared_ptr<PackageIdentifier> CompilationUnit::getPackageIdentifier()
{
    return package;
}

void CompilationUnit::addImport(shared_ptr<PackageIdentifier> import)
{
    imports.push_back(import);
}

vector<shared_ptr<PackageIdentifier> > CompilationUnit::getImports()
{
    return imports;
}

shared_ptr<Definition> CompilationUnit::addDefinition(shared_ptr<Definition> definition)
{
    definitions.push_back(definition);
    return definition;
}

vector<shared_ptr<Definition> > CompilationUnit::getDefinitions()
{
    return definitions;
}

shared_ptr<Definition> CompilationUnit::getDefinitionByIdentifier(shared_ptr<TokenIdentifier> identifier)
{
    for (auto definition : definitions)
    {
        if (definition->getIdentifier()->getName() == identifier->getName())
        {
            return definition;
        }
    }
    return nullptr;
}

string CompilationUnit::getDebug(string indent, bool last)
{
    string output = indent;

    if (last)
    {
        output += "└── ";
        indent += "    ";
    }
    else
    {
        output += "├── ";
        indent += "│   ";
    }

    output += ("[CompilationUnit] " + filename + "\n");

    bool hasDefinitions = !definitions.empty();

    for (size_t i = 0; i < imports.size(); i++)
    {
        bool isLast = (i + 1 == imports.size());
        if (hasDefinitions) isLast = false;

        auto import = imports[i];
        output += import->getDebug(indent, isLast);
    }

    for (size_t i = 0; i < definitions.size(); i++)
    {
        bool isLast = (i + 1 == definitions.size());

        auto def = definitions[i];
        output += def->getDebug(indent, isLast);
    }

    return output;
}