#include "BlockStatement.h"

vector<shared_ptr<Statement>> BlockStatement::getStatements()
{
    return statements;
}

void BlockStatement::addStatement(shared_ptr<Statement> statement)
{
    this->statements.push_back(statement);
}

void BlockStatement::addStatements(vector<shared_ptr<Statement>> statements)
{
    for (auto stat : statements)
    {
        addStatement(stat);
    }
}

string BlockStatement::getDebug(string indent, bool last, const string &custom)
{
    string output = Statement::getDebug(indent, last, custom);

    if (last)
    {
        indent += "    ";
    }
    else
    {
        indent += "│   ";
    }

    for (size_t i = 0; i < statements.size(); i++)
    {
        bool isLast = (i + 1 == statements.size());

        auto statement = statements[i];
        output += statement->getDebug(indent, isLast);
    }

    return output;
}