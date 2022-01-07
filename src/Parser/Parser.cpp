#include "Parser.h"
#include "../Package/Package.h"

Parser::Parser(ErrorHandler &error_handler, Lexer *lexer) : error_handler(error_handler)
{
    this->lexer = lexer;

    shared_ptr<Token> token = nullptr;
    while ((token = lexer->getNextToken()))
    {
        tokens.push_back(token);

        // error_handler.logLocation(E_DEBUG, token->location, token->getDebug().c_str());
    }

    if (!tokens.empty())
    {
        current_token = tokens[current_token_idx];
    }

    while (skipComment());
}

Parser::~Parser()
{
}

void Parser::debugCurrentToken()
{
    if (!current_token) return;
    if (current_token->getType() == TOKEN_COMMENT) return;

    error_handler.logLocation(
        E_DEBUG,
        current_token->location,
        current_token->getDebug().c_str()
    );
}

bool Parser::skipComment()
{
    if (!current_token) return false;

    if (current_token->getType() == TOKEN_COMMENT)
    {
        nextToken();
        return true;
    }
    return false;
}

void Parser::nextToken(size_t amount, bool debug)
{
    if (debug) debugCurrentToken();

    current_token_idx += amount;
    if (current_token_idx >= tokens.size())
    {
        current_token = nullptr;
    }
    else
    {
        current_token = tokens[current_token_idx];
    }

    while (skipComment());
}

void Parser::saveState(size_t slot)
{
    saved_state_token_idx[slot] = current_token_idx;

    error_handler.logLocation(
        E_DEBUG,
        current_token->location,
        "Saved Parser state"
    );
}

void Parser::loadState(size_t slot)
{
    current_token_idx = saved_state_token_idx[slot];
    clearState(slot);
    nextToken(0, false);

    error_handler.logLocation(
        E_DEBUG,
        current_token->location,
        "Loaded Parser state"
    );
}

void Parser::clearState(size_t slot)
{
    saved_state_token_idx[slot] = 0;
}

Location Parser::getCurrentTokenLocation()
{
    if (current_token) return current_token->location;
    return lexer->getLocation();
}

string Parser::getCurrentTokenDescription()
{
    if (current_token) return current_token->getDescription();
    return "end of file";
}

bool Parser::expect(TokenType token_type, bool report_error, string custom_message)
{
    if (current_token && current_token->getType() == token_type)
    {
        return true;
    }
    else if (report_error)
    {
        if (!custom_message.empty())
        {
            error_handler.throw_unexpected_token(
                current_token->location,
                custom_message
            );
        }
        else
        {
            string unexpected = getCurrentTokenDescription();
            string expected = Token::getDescription(token_type);

            error_handler.throw_unexpected_token(
                getCurrentTokenLocation(),
                unexpected,
                expected
            );
        }
    }
    return false;
}

shared_ptr<Token> Parser::eat(TokenType token_type, bool report_error, const string &custom_message)
{
    if (expect(token_type, report_error, custom_message))
    {
        auto eaten = current_token;
        nextToken();
        return eaten;
    }
    return nullptr;
}

vector<shared_ptr<Expression>> Parser::getExpressionList(bool required)
{
    vector<shared_ptr<Expression>> exprs;
    shared_ptr<Expression> expr;

    bool expectNext = required;
    while ((expr = getExpression(expectNext)))
    {
        exprs.push_back(expr);

        expectNext = (bool) eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_COMMA);
    }

    return exprs;
}

shared_ptr<Expression> Parser::getExpression(bool required, size_t op_lvl)
{
    shared_ptr<Expression> e;
    while (true)
    {
        if ((e = getParenExpression())) break;
        if ((e = getLiteralExpression())) break;
        if ((e = getIdentifierExpression())) break;
        if ((e = getNewStatementExpression())) break;
        if (op_lvl <= 9 && (e = getPreOperator())) break;

        if (!e && required)
        {
            error_handler.throw_unexpected_token(
                getCurrentTokenLocation(),
                "expected expression"
            );
        }
        return nullptr;
    }

    shared_ptr<OperationExpression> m;
    while (true)
    {
        if (op_lvl <= 1 && (m = getTernaryPart()))
        {
            m->setCondition(e);
            e = m;
            continue;
        }
        if (
            (
                (op_lvl <= 2  && (m = getAssignPart())) ||
                (op_lvl <= 3  && (m = getLogicPart())) ||
                (op_lvl <= 4  && (m = getBitwiseOpPart())) ||
                (op_lvl <= 5  && (m = getCmpPart())) ||
                (op_lvl <= 6  && (m = getBitwiseShiftPart())) ||
                (op_lvl <= 7  && (m = getTermPart())) ||
                (op_lvl <= 8  && (m = getFactorPart())) ||
                (op_lvl <= 10 && (m = getPostOperatorPart()))
            )
        )
        {
            m->setLeftExpression(e);
            e = m;
            continue;
        }
        if (
            op_lvl <= 10 &&
            (
                (m = getMemberAccessExpressionPart()) ||
                (m = getFunctionCallExpressionPart()) ||
                (m = getIndexExpressionPart())
            )
        )
        {
            m->setLeftExpression(e);
            e = m;
            continue;
        }
        break;
    }

    return e;
}

shared_ptr<LiteralExpression> Parser::getLiteralExpression(bool required)
{
    auto literal = getLiteral(required);
    if (literal)
    {
        return make_shared<LiteralExpression>(LiteralExpression(literal));
    }
    return nullptr;
}

shared_ptr<NewStatementExpression> Parser::getNewStatementExpression(bool required)
{
    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_NEW, required))
    {
        auto expr = getExpression(true);
        return make_shared<NewStatementExpression>(NewStatementExpression(expr));
    }
    return nullptr;
}

shared_ptr<Expression> Parser::getIdentifierExpression(bool required)
{
    auto ident = getIdentifier(required);
    if (ident)
    {
        return make_shared<IdentifierExpression>(IdentifierExpression(ident));
    }
    return nullptr;
}

shared_ptr<ParenExpression> Parser::getParenExpression(bool required)
{
    if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L, required))
    {
        auto expr = getExpression(true);
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);

        return make_shared<ParenExpression>(ParenExpression(expr));
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getFunctionCallExpressionPart(bool required)
{
    if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L, required))
    {
        auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_CALL));

        if (!eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R))
        {
            auto exprs = getExpressionList();
            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);
            o->addCallArgs(exprs);
        }

        return o;
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getIndexExpressionPart(bool required)
{
    if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_L, required))
    {
        auto expr = getExpression(true);
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_R, true);

        auto o = make_shared<OperationExpression>(OperationExpression());
        o->setKind(EXPRESSION_OPERATION_INDEX);
        o->setRightExpression(expr);
        return o;
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getMemberAccessExpressionPart(bool required)
{
    if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_DOT, required))
    {
        auto ident = getIdentifierExpression();

        auto o = make_shared<OperationExpression>(OperationExpression());
        o->setKind(EXPRESSION_OPERATION_MEMBER_ACCESS);
        o->setRightExpression(ident);
        return o;
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getPostOperatorPart(bool required)
{
    if (expect(TOKEN_OPERATOR, required))
    {
        auto op = static_pointer_cast<TokenOperator>(current_token);
        if (
            op &&
            (
                op->getKind() == TOKEN_OPERATOR_ARITH_INCR ||
                op->getKind() == TOKEN_OPERATOR_ARITH_DECR
            )
        )
        {
            eat(TOKEN_OPERATOR, true);
            auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_UNARY));
            o->setOperator(op);
            return o;
        }
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getPreOperator(bool required)
{
    if (expect(TOKEN_OPERATOR, required))
    {
        auto op = static_pointer_cast<TokenOperator>(current_token);
        if (
            op &&
            (
                op->getKind() == TOKEN_OPERATOR_ARITH_INCR ||
                op->getKind() == TOKEN_OPERATOR_ARITH_DECR ||
                op->getKind() == TOKEN_OPERATOR_ARITH_ADD ||
                op->getKind() == TOKEN_OPERATOR_ARITH_SUB ||
                op->getKind() == TOKEN_OPERATOR_ARITH_BIT_NOT ||
                op->getKind() == TOKEN_OPERATOR_LOGIC_NOT
            )
        )
        {
            eat(TOKEN_OPERATOR, true);
            auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_UNARY));

            auto expr = getExpression(true);
            o->setOperator(op);
            o->setRightExpression(expr);
            return o;
        }
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getFactorPart(bool required)
{
    if (expect(TOKEN_OPERATOR, required))
    {
        auto op = static_pointer_cast<TokenOperator>(current_token);
        if (
            op &&
            (
                op->getKind() == TOKEN_OPERATOR_ARITH_MULT ||
                op->getKind() == TOKEN_OPERATOR_ARITH_DIV ||
                op->getKind() == TOKEN_OPERATOR_ARITH_MOD
            )
        )
        {
            eat(TOKEN_OPERATOR, true);
            auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_BINARY));

            auto expr = getExpression(true, 8);
            o->setOperator(op);
            o->setRightExpression(expr);
            return o;
        }
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getTermPart(bool required)
{
    if (expect(TOKEN_OPERATOR, required))
    {
        auto op = static_pointer_cast<TokenOperator>(current_token);
        if (
            op &&
            (
                op->getKind() == TOKEN_OPERATOR_ARITH_ADD ||
                op->getKind() == TOKEN_OPERATOR_ARITH_SUB
            )
        )
        {
            eat(TOKEN_OPERATOR, true);
            auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_BINARY));

            auto expr = getExpression(true, 7);
            o->setOperator(op);
            o->setRightExpression(expr);
            return o;
        }
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getBitwiseShiftPart(bool required)
{
    if (expect(TOKEN_OPERATOR, required))
    {
        auto op = static_pointer_cast<TokenOperator>(current_token);
        if (
            op &&
            (
                op->getKind() == TOKEN_OPERATOR_ARITH_BIT_LSHIFT ||
                op->getKind() == TOKEN_OPERATOR_ARITH_BIT_RSHIFT
            )
        )
        {
            eat(TOKEN_OPERATOR, true);
            auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_BINARY));

            auto expr = getExpression(true, 6);
            o->setOperator(op);
            o->setRightExpression(expr);
            return o;
        }
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getCmpPart(bool required)
{
    if (expect(TOKEN_OPERATOR, required))
    {
        auto op = static_pointer_cast<TokenOperator>(current_token);
        if (
            op &&
            (
                op->getKind() >= TOKEN_OPERATOR_CMP_EQ &&
                op->getKind() <= TOKEN_OPERATOR_CMP_LESS_EQ
            )
        )
        {
            eat(TOKEN_OPERATOR, true);
            auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_BINARY));

            auto expr = getExpression(true, 5);
            o->setOperator(op);
            o->setRightExpression(expr);
            return o;
        }
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getBitwiseOpPart(bool required)
{
    if (expect(TOKEN_OPERATOR, required))
    {
        auto op = static_pointer_cast<TokenOperator>(current_token);
        if (
            op &&
            (
                op->getKind() >= TOKEN_OPERATOR_ARITH_BIT_NOT &&
                op->getKind() <= TOKEN_OPERATOR_ARITH_BIT_XOR
            )
        )
        {
            eat(TOKEN_OPERATOR, true);
            auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_BINARY));

            auto expr = getExpression(true, 4);
            o->setOperator(op);
            o->setRightExpression(expr);
            return o;
        }
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getLogicPart(bool required)
{
    if (expect(TOKEN_OPERATOR, required))
    {
        auto op = static_pointer_cast<TokenOperator>(current_token);
        if (
            op &&
            (
                op->getKind() == TOKEN_OPERATOR_LOGIC_OR ||
                op->getKind() == TOKEN_OPERATOR_LOGIC_AND
            )
        )
        {
            eat(TOKEN_OPERATOR, true);
            auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_BINARY));

            auto expr = getExpression(true, 3);
            o->setOperator(op);
            o->setRightExpression(expr);
            return o;
        }
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getAssignPart(bool required)
{
    if (expect(TOKEN_OPERATOR, required))
    {
        auto op = static_pointer_cast<TokenOperator>(current_token);
        if (
            op &&
            (
                (op->getKind() >= TOKEN_OPERATOR_ASSIGN) &&
                (op->getKind() <= TOKEN_OPERATOR_ASSIGN_BIT_RSHIFT)
            )
        )
        {
            eat(TOKEN_OPERATOR, true);
            auto o = make_shared<OperationExpression>(OperationExpression(EXPRESSION_OPERATION_BINARY));

            auto expr = getExpression(true, 2);
            o->setOperator(op);
            o->setRightExpression(expr);
            return o;
        }
    }
    return nullptr;
}

shared_ptr<OperationExpression> Parser::getTernaryPart(bool required)
{
    if (eatKind(TOKEN_OPERATOR, TOKEN_OPERATOR_LOGIC_TERNARY, required))
    {
        auto o = make_shared<OperationExpression>(OperationExpression());

        auto left = getExpression(true, 1);
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_COLON, true);
        auto right = getExpression(true, 1);

        o->setKind(EXPRESSION_OPERATION_TERNARY);
        o->setLeftExpression(left);
        o->setRightExpression(right);
        return o;
    }
    return nullptr;
}

shared_ptr<Statement> Parser::getStatement(bool required)
{
    shared_ptr<Statement> s = nullptr;

    if ((s = getEmptyStatement())) return s;
    if ((s = getBlockStatement())) return s;
    if ((s = getIfStatement())) return s;
    if ((s = getLoopStatement())) return s;
    if ((s = getFlowControlStatement())) return s;
    if ((s = getTryCatchStatement())) return s;
    if ((s = getVariableDefinitionStatement())) return s;
    if ((s = getExpressionStatement())) return s;
    if (!s && required)
    {
        error_handler.throw_unexpected_token(
            getCurrentTokenLocation(),
            "expected statement"
        );
    }
    return nullptr;
}

shared_ptr<VariableDefinitionStatement> Parser::getVariableDefinitionStatement(bool required)
{
    auto def = getVariableDefinition(required);
    if (def)
    {
        return make_shared<VariableDefinitionStatement>(VariableDefinitionStatement(def));
    }
    return nullptr;
}

shared_ptr<ExpressionStatement> Parser::getExpressionStatement(bool required)
{
    auto expr = getExpression(required);
    if (expr)
    {
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);
        return make_shared<ExpressionStatement>(ExpressionStatement(expr));
    }
    return nullptr;
}

shared_ptr<BlockStatement> Parser::getBlockStatement(bool required)
{
    if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_CURLY_L, required))
    {
        auto block = make_shared<BlockStatement>(BlockStatement());

        shared_ptr<Statement> s;
        while ((s = getStatement()))
        {
            block->addStatement(s);
        }

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_CURLY_R, true);

        return block;
    }
    return nullptr;
}

shared_ptr<IfStatement> Parser::getIfStatement(bool required)
{
    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_IF, required))
    {
        auto sif = make_shared<IfStatement>(IfStatement());

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L, true);

        sif->setCondition(getExpression(true));

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);

        sif->setBody(getStatement(true));

        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_ELSE))
        {
            sif->setElseBody(getStatement(true));
        }

        return sif;
    }
    return nullptr;
}

shared_ptr<LoopStatement> Parser::getLoopStatement(bool required)
{
    StatementLoopType loop_type = (StatementLoopType) (-1);

    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_FOR)) loop_type = STATEMENT_LOOP_FOR;
    else if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_WHILE)) loop_type = STATEMENT_LOOP_WHILE;
    else if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_DO)) loop_type = STATEMENT_LOOP_DO_WHILE;
    else if (required)
    {
        error_handler.throw_unexpected_token(
            getCurrentTokenLocation(),
            "expected loop keyword"
        );
    }
    else return nullptr;

    auto loop = make_shared<LoopStatement>(LoopStatement(loop_type));

    if (loop_type == STATEMENT_LOOP_DO_WHILE)
    {
        loop->setBody(getStatement(true));

        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_WHILE))
        {
            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L, true);

            loop->setCondition(getExpression(true));

            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);
            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);
        }
    }
    else
    {
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L, true);

        if (loop_type == STATEMENT_LOOP_FOR)
        {
            shared_ptr<Statement> def = getVariableDefinitionStatement();
            if (!def) def = getExpressionStatement(true);

            loop->setForInit(def);
        }

        loop->setCondition(getExpression(true));
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);

        if (loop_type == STATEMENT_LOOP_FOR)
        {
            loop->setForIter(getExpression(true));
        }

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);

        loop->setBody(getStatement(true));
    }

    return loop;
}

shared_ptr<FlowControlStatement> Parser::getFlowControlStatement(bool required)
{
    StatementFlowControlType flow_type = (StatementFlowControlType) (-1);

    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_BREAK)) flow_type = STATEMENT_FLOW_CONTROL_BREAK;
    else if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_CONTINUE)) flow_type = STATEMENT_FLOW_CONTROL_CONTINUE;
    else if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_RETURN)) flow_type = STATEMENT_FLOW_CONTROL_RETURN;
    else if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_THROW)) flow_type = STATEMENT_FLOW_CONTROL_THROW;
    else if (required)
    {
        error_handler.throw_unexpected_token(
            getCurrentTokenLocation(),
            "expected flow control statement"
        );
    }
    else return nullptr;

    auto flow = make_shared<FlowControlStatement>(FlowControlStatement(flow_type));

    if (flow_type == STATEMENT_FLOW_CONTROL_RETURN)
    {
        auto stat = getExpressionStatement();
        if (stat)
        {
            flow->setExpression(stat->getExpression());
        }
    }
    else
    {
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);
    }

    return flow;
}

shared_ptr<TryStatement> Parser::getTryCatchStatement(bool required)
{
    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_TRY, required))
    {
        auto stry = make_shared<TryStatement>(TryStatement());

        stry->setBody(getStatement(true));

        shared_ptr<CatchStatement> scatch;
        while ((scatch = getCatchStatement()))
        {
            stry->addCatchBlock(scatch);
        }

        return stry;
    }
    return nullptr;
}

shared_ptr<CatchStatement> Parser::getCatchStatement(bool required)
{
    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_CATCH, required))
    {
        auto scatch = make_shared<CatchStatement>(CatchStatement());

        if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L))
        {
            scatch->setCatchError(getFunctionParameter(true));
            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);
        }

        scatch->setBody(getStatement(true));

        return scatch;
    }
    return nullptr;
}

shared_ptr<EmptyStatement> Parser::getEmptyStatement(bool required)
{
    if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, required))
    {
        return make_shared<EmptyStatement>(EmptyStatement());
    }
    return nullptr;
}

shared_ptr<TokenLiteral> Parser::getLiteral(bool required, const string &custom_message)
{
    auto eaten = eat(TOKEN_LITERAL, required, custom_message);
    shared_ptr<TokenLiteral> literal = static_pointer_cast<TokenLiteral>(eaten);
    return literal;
}

shared_ptr<TokenIdentifier> Parser::getIdentifier(bool required, const string &custom_message)
{
    auto eaten = eat(TOKEN_IDENTIFIER, required, custom_message);
    shared_ptr<TokenIdentifier> ident = static_pointer_cast<TokenIdentifier>(eaten);
    return ident;
}

vector<shared_ptr<TokenIdentifier>> Parser::getIdentifierList(bool required)
{
    vector<shared_ptr<TokenIdentifier>> idents;
    shared_ptr<TokenIdentifier> ident;

    bool expectNext = required;
    while ((ident = getIdentifier(expectNext)))
    {
        idents.push_back(ident);

        expectNext = (bool) eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_COMMA);
    }

    return idents;
}

shared_ptr<TokenKeyword> Parser::getKeyword(bool required, const string &custom_message)
{
    auto eaten = eat(TOKEN_KEYWORD, required, custom_message);
    shared_ptr<TokenKeyword> keyw = static_pointer_cast<TokenKeyword>(eaten);
    return keyw;
}

shared_ptr<Modifiers> Parser::getModifiers()
{
    shared_ptr<Modifiers> modifiers = make_shared<Modifiers>(Modifiers());

    while (true)
    {
        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_PUBLIC))
        {
            modifiers->setAccess(MODIFIER_ACCESS_PUBLIC);
            continue;
        }
        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_PROTECTED))
        {
            modifiers->setAccess(MODIFIER_ACCESS_PROTECTED);
            continue;
        }
        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_PRIVATE))
        {
            modifiers->setAccess(MODIFIER_ACCESS_PRIVATE);
            continue;
        }
        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_STATIC))
        {
            modifiers->setStatic(true);
            continue;
        }
        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_NATIVE))
        {
            modifiers->setNative(true);
            continue;
        }
        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_ENTRYPOINT))
        {
            modifiers->setEntrypoint(true);
            continue;
        }
        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_ABSTRACT))
        {
            modifiers->setAbstract(true);
            continue;
        }
        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_FINAL))
        {
            modifiers->setFinal(true);
            continue;
        }
        break;
    }

    return modifiers;
}

shared_ptr<DataType> Parser::getDataType(bool required)
{
    saveState(2);
    if (expect(TOKEN_KEYWORD))
    {
        auto keyw = static_pointer_cast<TokenKeyword>(current_token);
        if (
            keyw->getKind() >= TOKEN_KEYWORD_VOID &&
            keyw->getKind() <= TOKEN_KEYWORD_DOUBLE
        )
        {
            eat(TOKEN_KEYWORD, true);
            auto type = make_shared<DataType>(DataType(keyw));

            while (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_L))
            {
                eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_R, true);
                type->addArrayDimension();
            }

            return type;
        }
    }
    else if (expect(TOKEN_IDENTIFIER))
    {
        auto ident = getIdentifier(true);
        auto type = make_shared<DataType>(DataType(ident));

        while (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_L))
        {
            if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_R))
            {
                type->addArrayDimension();
            }
            else if (!required)
            {
                loadState(2);
                return nullptr;
            }
        }

        clearState(2);
        return type;
    }
    else if (required)
    {
        error_handler.throw_unexpected_token(
            getCurrentTokenLocation(),
            "expected data type"
        );
    }
    return nullptr;
}

shared_ptr<VariableDefinition> Parser::getFunctionParameter(bool required)
{
    auto mod = getModifiers();
    auto datatype = getDataType(required);

    if (!datatype) return nullptr;

    auto ident = getIdentifier(true);

    auto def = make_shared<VariableDefinition>(VariableDefinition(datatype, ident));
    def->setModifiers(mod);

    if (eatKind(TOKEN_OPERATOR, TOKEN_OPERATOR_ASSIGN))
    {
        def->setDefaultValue(getExpression(true));
    }

    return def;
}

vector<shared_ptr<VariableDefinition>> Parser::getFunctionParameterList()
{
    vector<shared_ptr<VariableDefinition>> params;

    bool expectNext = false;
    shared_ptr<VariableDefinition> param;
    while ((param = getFunctionParameter(expectNext)))
    {
        params.push_back(param);

        expectNext = (bool) eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_COMMA);
    }

    return params;
}

shared_ptr<TokenOperator> Parser::getOperator(bool required)
{
    auto eaten = eat(TOKEN_OPERATOR, required);
    shared_ptr<TokenOperator> op = static_pointer_cast<TokenOperator>(eaten);
    return op;
}

shared_ptr<PackageIdentifier> Parser::getPackageIdentifier()
{
    auto ident = make_shared<PackageIdentifier>(PackageIdentifier());

    ident->addIdentifier(getIdentifier(true));

    while (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_DOT))
    {
        if (eatKind(TOKEN_OPERATOR, TOKEN_OPERATOR_ARITH_MULT))
        {
            ident->setWildcard(true);
            break;
        }
        else
        {
            ident->addIdentifier(getIdentifier(true));
        }
    }

    eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);

    return ident;
}

shared_ptr<PackageIdentifier> Parser::getPackageDefinition()
{
    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_PACKAGE))
    {
        return getPackageIdentifier();
    }
    return make_shared<PackageIdentifier>(PackageIdentifier());
}

shared_ptr<PackageIdentifier> Parser::getImportStatement()
{
    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_IMPORT))
    {
        return getPackageIdentifier();
    }
    return nullptr;
}

shared_ptr<Definition> Parser::getDefinition(bool required)
{
    auto mod = getModifiers();

    shared_ptr<Definition> def = nullptr;
    if ((def = getClassDefinition())) return def;
    if ((def = getFunctionDefinition())) return def;
    if ((def = getVariableDefinition())) return def;
    if ((def = getAliasDefinition())) return def;

    // TODO: getEnumDefinition()
    // if ((def = getEnumDefinition())) return def;

    if (!def && required)
    {
        error_handler.throw_unexpected_token(
            getCurrentTokenLocation(),
            "expected definition"
        );
    }
    else if (def)
    {
        def->setModifiers(mod);
    }

    return def;
}

shared_ptr<ClassDefinition> Parser::getClassDefinition()
{
    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_CLASS))
    {
        auto ident = getIdentifier(true);

        auto class_def = make_shared<ClassDefinition>(ClassDefinition(ident));

        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_EXTENDS))
        {
            class_def->addExtends(getIdentifierList(true));
        }

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_CURLY_L, true);

        shared_ptr<Definition> def;
        while ((def = getDefinition()))
        {
            class_def->addDefinition(def);
        }

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_CURLY_R, true);

        return class_def;
    }
    return nullptr;
}

shared_ptr<VariableDefinition> Parser::getVariableDefinition(bool required)
{
    saveState(1);
    auto datatype = getDataType(required);
    if (datatype)
    {
        auto ident = getIdentifier(required);
        if (ident)
        {
            auto def = make_shared<VariableDefinition>(VariableDefinition(datatype, ident));

            if (eatKind(TOKEN_OPERATOR, TOKEN_OPERATOR_ASSIGN))
            {
                def->setDefaultValue(getExpression(true));
            }

            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);

            clearState(1);
            return def;
        }
    }
    loadState(1);
    return nullptr;
}

shared_ptr<FunctionDefinition> Parser::getFunctionDefinition()
{
    saveState(1);
    auto datatype = getDataType();
    if (datatype)
    {
        auto ident = getIdentifier();
        if (ident)
        {
            if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L))
            {
                auto def = make_shared<FunctionDefinition>(FunctionDefinition(datatype, ident));

                def->addParameters(getFunctionParameterList());

                eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);

                if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_THROWS))
                {
                    def->addThrows(getIdentifierList(true));
                }

                def->setBody(getBlockStatement(true));

                clearState(1);
                return def;
            }
        }
    }
    loadState(1);
    return nullptr;
}

shared_ptr<AliasDefinition> Parser::getAliasDefinition()
{
    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_ALIAS))
    {
        auto id1 = getIdentifier(true);
        eatKind(TOKEN_OPERATOR, TOKEN_OPERATOR_ASSIGN, true);
        auto id2 = getDataType(true);
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);

        auto def = make_shared<AliasDefinition>(AliasDefinition(id1, id2));
        return def;
    }
    return nullptr;
}

shared_ptr<CompilationUnit> Parser::parse()
{
    if (!tokens.empty() && tokens[0]->getType() == TOKEN_COMMENT)
    {
        shared_ptr<TokenComment> comment = static_pointer_cast<TokenComment>(tokens[0]);
        if (comment->getContent() == " fortal::ignore_parser ")
        {
            return nullptr;
        }
    }

    string filename = lexer->getFilename();

    auto unit = make_shared<CompilationUnit>(CompilationUnit(getPackageDefinition(), filename));

    shared_ptr<PackageIdentifier> import;
    while ((import = getImportStatement()))
    {
        unit->addImport(import);
    }

    while (current_token)
    {
        unit->addDefinition(getDefinition(true));
    }

    return unit;
}