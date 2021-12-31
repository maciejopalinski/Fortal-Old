#include "Compiler.h"
#include "../Lexer/Lexer.h"

Compiler::Compiler(ErrorHandler &error_handler) : error_handler(error_handler) {}

void Compiler::addFile(string filename)
{
    files.push_back(filename);
}

void Compiler::compile()
{
    for (auto filename : files)
    {
        compile(filename);
    }

    error_handler.log(E_EXTRA, "Compilation finished!");

    error_handler.log(E_EXTRA, "\n%s", global_package->getDebug().c_str());
}

void Compiler::compile(string filename)
{
    error_handler.log(E_DEBUG, "Compiling '%s'...", filename.c_str());

    Lexer lexer(error_handler);
    lexer.loadFromFile(filename.c_str());

    Parser parser(error_handler, &lexer);

    auto unit = parser.parse();
    global_package->addCompilationUnit(unit);

    error_handler.log(E_EXTRA, "Compiled '%s'", filename.c_str());
}