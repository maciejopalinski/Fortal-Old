#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

#include "../ErrorHandler/ErrorHandler.h"
#include "../Package/Package.h"
#include "../Parser/Parser.h"

class Compiler
{
    private:
        ErrorHandler &error_handler;
        shared_ptr<Package> global_package = make_shared<Package>();

        vector<string> files;

    public:
        Compiler(ErrorHandler &error_handler);

        void addFile(string filename);

        void compile();
        void compile(string filename);
};