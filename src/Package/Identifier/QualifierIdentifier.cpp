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

string QualifiedIdentifier::getDebug(string indent, string name, bool last)
{
    string output = indent;

    if (last)
    {
        output += "└── ";
    }
    else
    {
        output += "├── ";
    }

    output +=
        format(
            "[%s] ",
            name.c_str()
        ) + getQualifiedIdentifier() + "\n";

    return output;
}