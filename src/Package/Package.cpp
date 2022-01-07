#include "Package.h"
#include <stdexcept>

Package::Package(const string &full_name)
{
    Package(split_string(full_name, "."));
}

Package::Package(vector<string> name_vector)
{
    if (name_vector.empty())
    {
        throw std::invalid_argument("Package identifier cannot be empty!");
    }

    this->name_vector = name_vector;
    this->full_name = join_string(name_vector, ".");
    this->name = name_vector.back();
}

void Package::addChildPackage(const string &package_name)
{
    addChildPackage(split_string(package_name, "."));
}

void Package::addChildPackage(vector<string> name_vector, size_t step)
{
    if (!name_vector.empty())
    {
        if (step >= name_vector.size()) return;

        auto child = getChildPackageByName(name_vector[step]);
        if (!child)
        {
            vector<string> child_v(name_vector);
            while (child_v.size() != (step + 1))
            {
                child_v.pop_back();
            }

            child = make_shared<Package>(child_v);
            child_packages.push_back(child);
        }

        child->addChildPackage(name_vector, step + 1);
    }
}

shared_ptr<Package> Package::getChildPackageByName(const string &full_name)
{
    return getChildPackageByName(split_string(full_name, "."));
}

shared_ptr<Package> Package::getChildPackageByName(vector<string> name_vector)
{
    if (!name_vector.empty())
    {
        for (auto package : child_packages)
        {
            if (package->name == name_vector[0])
            {
                if (name_vector.size() == 1)
                {
                    return package;
                }
                else
                {
                    name_vector.erase(name_vector.begin());
                    return package->getChildPackageByName(name_vector);
                }
            }
        }
    }
    return nullptr;
}

vector<shared_ptr<Package> > Package::getChildPackages()
{
    return child_packages;
}

shared_ptr<CompilationUnit> Package::addCompilationUnit(shared_ptr<CompilationUnit> unit)
{
    if (!unit) return nullptr;

    string ident = unit->getPackageIdentifier()->getQualifiedIdentifier();

    if (ident == this->full_name)
    {
        this->compilation_units.push_back(unit);
        return unit;
    }
    else
    {
        addChildPackage(ident);

        auto package = getChildPackageByName(ident);
        if (package)
        {
            return package->addCompilationUnit(unit);
        }
    }
    return nullptr;
}

vector<shared_ptr<CompilationUnit> > Package::getCompilationUnits()
{
    return compilation_units;
}

string Package::getDebug(string indent, bool last)
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

    output += ("[Package] " + name + " (" + full_name + ")" + '\n');

    bool hasChildPackages = !child_packages.empty();

    for (size_t i = 0; i < compilation_units.size(); i++)
    {
        bool isLast = (i + 1 == compilation_units.size());
        if (hasChildPackages) isLast = false;

        auto unit = compilation_units[i];
        output += unit->getDebug(indent, isLast);
    }

    for (size_t i = 0; i < child_packages.size(); i++)
    {
        bool isLast = (i + 1 == child_packages.size());

        auto child_package = child_packages[i];
        output += child_package->getDebug(indent, isLast);
    }

    return output;
}