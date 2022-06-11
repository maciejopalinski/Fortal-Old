cmake_minimum_required(VERSION 3.5)

project(FortalCC
    VERSION 0.4.0
    DESCRIPTION "Compiler for Fortal programming language"
)

add_executable(Fortal
    src/ErrorHandler/ErrorLevel.cpp
    src/ErrorHandler/ErrorHandler.cpp
    src/Token/TokenHasKind.cpp
    src/Token/TokenLiteral.cpp
    src/Token/Token.cpp
    src/Token/TokenComment.cpp
    src/Token/TokenKeyword.cpp
    src/Token/TokenSeparator.cpp
    src/Token/TokenOperator.cpp
    src/Token/TokenIdentifier.cpp
    src/Package/Package.cpp
    src/Package/CompilationUnit/Definition/Expression/IdentifierExpression.cpp
    src/Package/CompilationUnit/Definition/Expression/LiteralExpression.cpp
    src/Package/CompilationUnit/Definition/Expression/OperationExpression.cpp
    src/Package/CompilationUnit/Definition/Expression/Expression.cpp
    src/Package/CompilationUnit/Definition/Expression/NewStatementExpression.cpp
    src/Package/CompilationUnit/Definition/Expression/ParenExpression.cpp
    src/Package/CompilationUnit/Definition/Modifiers/Modifiers.cpp
    src/Package/CompilationUnit/Definition/VariableDefinition.cpp
    src/Package/CompilationUnit/Definition/Statement/Statement.cpp
    src/Package/CompilationUnit/Definition/Statement/FlowControlStatement.cpp
    src/Package/CompilationUnit/Definition/Statement/LoopStatement.cpp
    src/Package/CompilationUnit/Definition/Statement/StatementHasBody.cpp
    src/Package/CompilationUnit/Definition/Statement/IfStatement.cpp
    src/Package/CompilationUnit/Definition/Statement/CatchStatement.cpp
    src/Package/CompilationUnit/Definition/Statement/VariableDefinitionStatement.cpp
    src/Package/CompilationUnit/Definition/Statement/EmptyStatement.cpp
    src/Package/CompilationUnit/Definition/Statement/TryStatement.cpp
    src/Package/CompilationUnit/Definition/Statement/BlockStatement.cpp
    src/Package/CompilationUnit/Definition/Statement/ExpressionStatement.cpp
    src/Package/CompilationUnit/Definition/Statement/StatementHasCondition.cpp
    src/Package/CompilationUnit/Definition/Definition.cpp
    src/Package/CompilationUnit/Definition/FunctionDefinition.cpp
    src/Package/CompilationUnit/Definition/AliasDefinition.cpp
    src/Package/CompilationUnit/Definition/ClassDefinition.cpp
    src/Package/CompilationUnit/Definition/DataType/DataType.cpp
    src/Package/CompilationUnit/CompilationUnit.cpp
    src/Package/Identifier/PackageIdentifier.cpp
    src/Package/Identifier/QualifierIdentifier.cpp
    src/main.cpp
    src/Lexer/Lexer.cpp
    src/Compiler/Compiler.cpp
    src/Parser/Parser.cpp
    src/Utils/Format.cpp
)

target_include_directories(Fortal PUBLIC include)

target_compile_options(Fortal PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)

add_executable(FortalTest
    test/main.cpp
)

target_link_libraries(FortalTest gtest)