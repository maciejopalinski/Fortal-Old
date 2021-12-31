#include "QualifiedIdentifier.h"
#include "../../Package/Package.h"

QualifiedIdentifier::~QualifiedIdentifier()
{
}

void QualifiedIdentifier::addIdentifier(shared_ptr<TokenIdentifier> token)
{
    identifiers.push_back(token);
}

vector<shared_ptr<TokenIdentifier>> QualifiedIdentifier::getIdentifiers()
{
    return identifiers;
}

string QualifiedIdentifier::getQualifiedIdentifier()
{
    vector<string> string_identifiers;
    for (auto token : identifiers)
    {
        string_identifiers.push_back(token->getName());
    }

    string out = join_string(string_identifiers, ".");

    if (out.empty()) out = ".";
    return out;
}

string QualifiedIdentifier::getDebug(string indent, bool last)
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

    output += ("[Import] " + getQualifiedIdentifier() + "\n");

    return output;
}