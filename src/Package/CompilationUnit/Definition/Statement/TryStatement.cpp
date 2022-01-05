#include "TryStatement.h"

vector<shared_ptr<CatchStatement>> TryStatement::getCatchBlocks()
{
    return catch_blocks;
}

void TryStatement::addCatchBlock(shared_ptr<CatchStatement> catch_block)
{
    catch_blocks.push_back(catch_block);
}

void TryStatement::addCatchBlocks(vector<shared_ptr<CatchStatement>> catch_blocks)
{
    for (auto block : catch_blocks)
    {
        addCatchBlock(block);
    }
}

string TryStatement::getDebug(string indent, bool last, string custom)
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

    last = catch_blocks.empty();
    output += getBody()->getDebug(indent, last);

    for (size_t i = 0; i < catch_blocks.size(); i++)
    {
        bool isLast = (i + 1 == catch_blocks.size());

        auto catch_block = catch_blocks[i];
        output += catch_block->getDebug(indent, isLast);
    }

    return output;
}
