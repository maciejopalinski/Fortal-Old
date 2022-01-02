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

    current_token_idx = 0;
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

void Parser::nextToken()
{
    debugCurrentToken();

    current_token_idx++;
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

bool Parser::expect(TokenType token_type, bool report_error, string custom_message)
{
    if (!current_token) return false;

    if (current_token->getType() == token_type)
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
            string unexpected = current_token->getDescription();
            string expected = Token::getDescription(token_type);

            error_handler.throw_unexpected_token(
                current_token->location,
                unexpected,
                expected
            );
        }
    }
    return false;
}

shared_ptr<Token> Parser::eat(TokenType token_type, bool report_error, string custom_message)
{
    if (expect(token_type, report_error, custom_message))
    {
        auto eaten = current_token;
        nextToken();
        return eaten;
    }
    return nullptr;
}

shared_ptr<Statement> Parser::getStatement(bool required)
{
    shared_ptr<Statement> s = nullptr;

    if ((s = getExpressionStatement())) return s;
    if ((s = getBlockStatement())) return s;
    if ((s = getIfStatement())) return s;
    if ((s = getLoopStatement())) return s;
    if ((s = getFlowControlStatement())) return s;
    if ((s = getTryCatchStatement())) return s;
    if ((s = getEmptyStatement())) return s;

    if (!s && required)
    {
        // TODO: something is hacky around here, maybe should be changed
        Location location;
        if (!current_token) location = this->lexer->getLocation();
        else location = current_token->location;

        error_handler.throw_unexpected_token(
            location,
            "expected statement"
        );
    }

    return s;
}

shared_ptr<ExpressionStatement> Parser::getExpressionStatement(bool required)
{
    // TODO: getExpression()
    // return getExpression(required);
    return required ? nullptr : nullptr;
}

shared_ptr<BlockStatement> Parser::getBlockStatement(bool required)
{
    if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_CURLY_L, required))
    {
        auto block =
            make_shared<BlockStatement>(
                BlockStatement()
            );

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
        auto sif =
            make_shared<IfStatement>(
                IfStatement()
            );

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L, true);

        // TODO: getExpression();
        // sif->setCondition(getExpression());

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);

        sif->setBody(getStatement(true));

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
    else
    {
        if (!required) return nullptr;

        // TODO: something is hacky around here, maybe should be changed
        Location location;
        if (!current_token) location = this->lexer->getLocation();
        else location = current_token->location;

        error_handler.throw_unexpected_token(
            location,
            "expected loop keyword"
        );
    }

    auto loop =
        make_shared<LoopStatement>(
            LoopStatement(loop_type)
        );

    if (loop_type == STATEMENT_LOOP_DO_WHILE)
    {
        loop->setBody(getStatement(true));

        if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_WHILE))
        {
            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L, true);

            // TODO: getExpression();
            // loop->setCondition(getExpression(true));

            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);
            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);
        }
    }
    else
    {
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L, true);

        if (loop_type == STATEMENT_LOOP_FOR)
        {
            // TODO: getExpression();
            // loop->setForInit(getExpression());
        }

        // TODO: getExpression();
        // loop->setCondition(getExpression(true));

        if (loop_type == STATEMENT_LOOP_FOR)
        {
            // TODO: getExpression();
            // loop->setForIter(getExpression());
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
    else
    {
        if (!required) return nullptr;

        // TODO: something is hacky around here, maybe should be changed
        Location location;
        if (!current_token) location = this->lexer->getLocation();
        else location = current_token->location;

        error_handler.throw_unexpected_token(
            location,
            "expected flow control statement"
        );
    }

    eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);

    return
        make_shared<FlowControlStatement>(
            FlowControlStatement(flow_type)
        );
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
        return
            make_shared<EmptyStatement>(EmptyStatement());
    }
    return nullptr;
}

shared_ptr<TokenIdentifier> Parser::getIdentifier(bool required, string custom_message)
{
    auto eaten = eat(TOKEN_IDENTIFIER, required, custom_message);
    shared_ptr<TokenIdentifier> ident = static_pointer_cast<TokenIdentifier>(eaten);
    return ident;
}

shared_ptr<TokenKeyword> Parser::getKeyword(bool required, string custom_message)
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
    if (expect(TOKEN_KEYWORD))
    {
        auto keyw = getKeyword();
        auto type = make_shared<DataType>(DataType(keyw));

        while (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_L))
        {
            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_R, true);
            type->addArrayDimension();
        }

        if (type->isValid()) return type;
    }
    else if (expect(TOKEN_IDENTIFIER))
    {
        auto ident = getIdentifier();
        auto type = make_shared<DataType>(DataType(ident));

        while (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_L))
        {
            eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_SQUARE_R, true);
            type->addArrayDimension();
        }

        return type;
    }
    else if (required)
    {
        // TODO: something is hacky around here, maybe should be changed
        Location location;
        if (!current_token) location = this->lexer->getLocation();
        else location = current_token->location;

        error_handler.throw_unexpected_token(
            location,
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

    auto def =
        make_shared<VariableDefinition>(
            VariableDefinition(mod, datatype, ident)
        );

    // TODO: getExpression();
    // def->setDefaultValue(getExpression(true));

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

        if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_COMMA))
        {
            expectNext = true;
        }
        else break;
    }

    return params;
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

shared_ptr<Definition> Parser::getDefinition()
{
    shared_ptr<Definition> def = nullptr;
    if ((def = getClassDefinition())) return def;
    if ((def = getFunctionOrVariableDefinition())) return def;

    // TODO: getEnumDefinition()
    // if ((def = getEnumDefinition())) return def;

    // TODO: getAliasDefinition()
    // if ((def = getAliasDefinition())) return def;

    return def;
}

shared_ptr<ClassDefinition> Parser::getClassDefinition()
{
    if (eatKind(TOKEN_KEYWORD, TOKEN_KEYWORD_CLASS))
    {
        auto mod = getModifiers();
        auto ident = getIdentifier(true);
        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);

        auto class_def =
            make_shared<ClassDefinition>(
                ClassDefinition(mod, ident)
            );

        return class_def;
    }
    return nullptr;
}

shared_ptr<Definition> Parser::getFunctionOrVariableDefinition()
{
    auto mod = getModifiers();
    auto datatype = getDataType();

    if (!datatype) return nullptr;

    auto ident = getIdentifier(true);

    if (expectKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L))
    {
        auto def =
            make_shared<FunctionDefinition>(
                FunctionDefinition(mod, datatype, ident)
            );

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_L, true);

        def->addParameters(getFunctionParameterList());

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);

        def->setBody(getBlockStatement(true));

        return def;
    }
    else
    {
        auto def =
            make_shared<VariableDefinition>(
                VariableDefinition(mod, datatype, ident)
            );

        if (eatKind(TOKEN_OPERATOR, TOKEN_OPERATOR_ASSIGN))
        {
            // TODO: getExpression()
            // def->setDefaultValue(getExpression(true));
        }

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_SEMICOLON, true);
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

    auto unit =
        make_shared<CompilationUnit>(
            CompilationUnit(getPackageDefinition(), filename)
        );

    shared_ptr<PackageIdentifier> import;
    while ((import = getImportStatement()))
    {
        unit->addImport(import);
    }

    shared_ptr<Definition> definition;
    while ((definition = getDefinition()))
    {
        unit->addDefinition(definition);
    }

    return unit;
}