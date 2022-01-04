#pragma once
#include <string>
#include <memory>
using std::string;
using std::static_pointer_cast;

#include "../ErrorHandler/ErrorHandler.h"
#include "../Lexer/Lexer.h"

#include "../Package/CompilationUnit/CompilationUnit.h"
#include "../Package/Identifier/PackageIdentifier.h"
#include "../Package/CompilationUnit/Definition/ClassDefinition.h"
#include "../Package/CompilationUnit/Definition/FunctionDefinition.h"
#include "../Package/CompilationUnit/Definition/VariableDefinition.h"

#include "../Package/CompilationUnit/Definition/Statement/Statements.all.h"

#include "../Package/CompilationUnit/Definition/Expression/Expressions.all.h"

class Parser
{
    private:
        ErrorHandler &error_handler;

        Lexer *lexer;

        shared_ptr<Token> current_token;
        vector<shared_ptr<Token>> tokens;
        size_t current_token_idx;

    public:
        Parser(ErrorHandler &error_handler, Lexer *lexer);
        ~Parser();

        void debugCurrentToken();
        bool skipComment();
        void nextToken();

        template<class T>
        bool expectKind(TokenType token_type, T token_kind, bool report_error = false, string custom_message = "");
        bool expect(TokenType token_type, bool report_error = false, string custom_message = "");

        template<class T>
        shared_ptr<TokenHasKind<T>> eatKind(TokenType token_type, T token_kind, bool report_error = false, string custom_message = "");
        shared_ptr<Token> eat(TokenType token_type, bool report_error = false, string custom_message = "");

        // EXPRESSION WRAPPER
        vector<shared_ptr<Expression>> getExpressionList(bool required = false);
        shared_ptr<Expression> getExpression(bool required = false, bool check_operators = true);

        // BASIC EXPRESSIONS
        shared_ptr<LiteralExpression> getLiteralExpression(bool required = false);
        shared_ptr<VariableDefinitionExpression> getVariableDefinitionExpression(bool required = false);
        shared_ptr<NewStatementExpression> getNewStatementExpression(bool required = false);
        shared_ptr<Expression> getIdentifierExpression(bool required = false);
        shared_ptr<ParenExpression> getParenExpression(bool required = false);

        // OPERATION EXPRESSIONS
        shared_ptr<OperationExpression> getFunctionCallExpressionPart(bool required = false);
        shared_ptr<OperationExpression> getIndexExpressionPart(bool required = false);
        shared_ptr<OperationExpression> getMemberAccessExpressionPart(bool required = false);
        shared_ptr<OperationExpression> getPostOperatorPart(bool required = false);
        shared_ptr<OperationExpression> getPreOperator(bool required = false);
        shared_ptr<OperationExpression> getFactorPart(bool required = false);
        shared_ptr<OperationExpression> getTermPart(bool required = false);
        shared_ptr<OperationExpression> getBitwiseShiftPart(bool required = false);
        shared_ptr<OperationExpression> getCmpPart(bool required = false);
        shared_ptr<OperationExpression> getBitwiseOpPart(bool required = false);
        shared_ptr<OperationExpression> getLogicPart(bool required = false);
        shared_ptr<OperationExpression> getAssignPart(bool required = false);
        shared_ptr<OperationExpression> getTernaryPart(bool required = false);

        // STATEMENT WRAPPER
        shared_ptr<Statement> getStatement(bool required = false);

        // BASIC STATEMENTS
        shared_ptr<ExpressionStatement> getExpressionStatement(bool required = false);
        shared_ptr<BlockStatement> getBlockStatement(bool required = false);
        shared_ptr<IfStatement> getIfStatement(bool required = false);
        shared_ptr<LoopStatement> getLoopStatement(bool required = false);
        shared_ptr<FlowControlStatement> getFlowControlStatement(bool required = false);
        shared_ptr<TryStatement> getTryCatchStatement(bool required = false);
        shared_ptr<CatchStatement> getCatchStatement(bool required = false);
        shared_ptr<EmptyStatement> getEmptyStatement(bool required = false);

        // TOKENS / UTILS
        shared_ptr<TokenLiteral> getLiteral(bool required = false, string custom_message = "");
        shared_ptr<TokenIdentifier> getIdentifier(bool required = false, string custom_message = "");
        vector<shared_ptr<TokenIdentifier>> getIdentifierList(bool required = false);
        shared_ptr<TokenKeyword> getKeyword(bool required = false, string custom_message = "");
        shared_ptr<Modifiers> getModifiers();
        shared_ptr<DataType> getDataType(bool required = false);
        shared_ptr<VariableDefinition> getFunctionParameter(bool required = false);
        vector<shared_ptr<VariableDefinition>> getFunctionParameterList();
        shared_ptr<TokenOperator> getOperator(bool required = false);

        // PACKAGE
        shared_ptr<PackageIdentifier> getPackageIdentifier();
        shared_ptr<PackageIdentifier> getPackageDefinition();
        shared_ptr<PackageIdentifier> getImportStatement();

        // COMPILATION UNIT
        shared_ptr<Definition> getDefinition(bool required = false);
        shared_ptr<ClassDefinition> getClassDefinition();
        shared_ptr<VariableDefinition> getVariableDefinition(bool required = false);
        shared_ptr<Definition> getFunctionOrVariableDefinition();

        shared_ptr<CompilationUnit> parse();
};

template<class T>
bool Parser::expectKind(TokenType token_type, T token_kind, bool report_error, string custom_message)
{
    if (!current_token)
    {
        if (report_error)
        {
            error_handler.throw_unexpected_eof(lexer->getLocation());
        }
        return false;
    }

    if (current_token->getType() == token_type)
    {
        shared_ptr<TokenHasKind<T>> token = static_pointer_cast<TokenHasKind<T>>(current_token);

        if (token->getKind() == token_kind)
        {
            return true;
        }
    }

    if (report_error)
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
            string expected = TokenHasKind<T>::getDescription(token_kind);

            error_handler.throw_unexpected_token(
                current_token->location,
                unexpected,
                expected
            );
        }
    }
    return false;
}

template<class T>
shared_ptr<TokenHasKind<T>> Parser::eatKind(TokenType token_type, T token_kind, bool report_error, string custom_message)
{
    if (expectKind(token_type, token_kind, report_error, custom_message))
    {
        shared_ptr<TokenHasKind<T>> eaten = static_pointer_cast<TokenHasKind<T>>(current_token);
        nextToken();
        return eaten;
    }
    return nullptr;
}