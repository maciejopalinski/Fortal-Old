#pragma once
#include <vector>
using std::vector;

#include "StatementHasBody.h"
#include "CatchStatement.h"

class TryStatement : public StatementHasBody
{
    private:
        vector<shared_ptr<CatchStatement>> catch_blocks;

    public:
        StatementType getType()
        {
            return STATEMENT_TRY;
        }

        vector<shared_ptr<CatchStatement>> getCatchBlocks();
        void addCatchBlock(shared_ptr<CatchStatement> catch_block);
        void addCatchBlocks(vector<shared_ptr<CatchStatement>> catch_blocks);

        string getDebug(string indent, bool last = true, string custom = "");
};