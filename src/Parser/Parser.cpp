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
    current_token = tokens[current_token_idx];

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

    // TODO: default value

    return def;
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
    // // if ((def = getEnumDefinition())) return def;
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


        bool expectNext = false;
        shared_ptr<VariableDefinition> param;
        while ((param = getFunctionParameter(expectNext)))
        {
            def->addParameter(param);

            if (eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_COMMA))
            {
                expectNext = true;
            }
            else break;
        }

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_PAREN_R, true);

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_CURLY_L, true);

        // TODO: function body

        eatKind(TOKEN_SEPARATOR, TOKEN_SEPARATOR_BRACKET_CURLY_R, true);

        return def;
    }
    else
    {
        auto def =
            make_shared<VariableDefinition>(
                VariableDefinition(mod, datatype, ident)
            );

        if (expectKind(TOKEN_OPERATOR, TOKEN_OPERATOR_ASSIGN))
        {
            // TODO: default value
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
            // error_handler.log(
            //     E_EXTRA,
            //     "Ignoring '%s'",
            //     this->lexer->getFilename().c_str()
            // );
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