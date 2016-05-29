// The MIT License (MIT)
// Copyright (c) 2016 Samuel Roth
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "lexer.h"
#include "parser.h"

IronLexer lexer;

int IronParser::getNextToken() {
    return this->currentToken = lexer.getToken();
}

std::unique_ptr<ExpressionAST> IronParser::ErrorHandler(const char *string) {
    fprintf(stderr, "ErrorHandler: %s\n", string);
    return nullptr;
}

std::unique_ptr<PrototypeAST> IronParser::ErrorHandlerP(const char *string) {
    this->ErrorHandler(string);
    return nullptr;
}

std::unique_ptr<ExpressionAST> IronParser::ParseNumber() {
    auto result = llvm::make_unique<NumberAST>(lexer.numericValue);
    this->getNextToken();
    return std::move(result);
}

std::unique_ptr<ExpressionAST> IronParser::ParseParenthesis() {
    this->getNextToken();
    auto v = this->ParseExpression();
    if (!v)
        return nullptr;
    if (this->currentToken != ')')
        return this->ErrorHandler("expected ')'");
    return v;
}

std::unique_ptr<ExpressionAST> IronParser::ParseIdentifier() {
    std::string identifier = lexer.identifier;
    this->getNextToken();
    if (this->currentToken != '(')
        return llvm::make_unique<VariableAST>(identifier);
    this->getNextToken();
    std::vector<std::unique_ptr<ExpressionAST>> arguments;
    if (this->currentToken != ')') {
        while (1) {
            if (auto argument = this->ParseExpression())
                arguments.push_back(std::move(argument));
            else
                return nullptr;
            if (this->currentToken == ')')
                break;
            if (this->currentToken != ',')
                return this->ErrorHandler("Expected ')' or ',' in argument list");
            this->getNextToken();
        }
    }

    this->getNextToken();
    return llvm::make_unique<FunctionCallAST>(identifier, std::move(arguments));
}

std::unique_ptr<ExpressionAST> IronParser::ParsePrimary() {
    switch(this->currentToken) {
        default:
            return ErrorHandler("unknown token when expecting an expression");
        case IronLexer::token_identifier:
            return ParseIdentifier();
        case IronLexer::token_number:
            return ParseNumber();
        case '(':
            return ParseParenthesis();
    }
}

std::unique_ptr<ExpressionAST> IronParser::ParseExpression() {
    auto LHS = this->ParsePrimary();
    if (!LHS)
        return nullptr;

    return this->ParseOperationRHS(0, std::move(LHS));
}

std::unique_ptr<ExpressionAST> IronParser::ParseOperationRHS(int precedence,
                                           std::unique_ptr<ExpressionAST> LHS) {
    while (1) {
        int tokenPrecedence = this->getTokenPrecedence();
        if (tokenPrecedence < precedence)
            return LHS;

        int binaryOperator = this->currentToken;
        this->getNextToken();

        auto RHS = this->ParsePrimary();
        if (!RHS)
            return nullptr;

        int nextPrecedence = this->getTokenPrecedence();
        if (tokenPrecedence < nextPrecedence) {
            RHS = this->ParseOperationRHS(tokenPrecedence+1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        LHS = helper::make_unique<BinaryOperatorAST>(binaryOperator,
                                                     std::move(LHS),
                                                     std::move(RHS));
    }
}

int IronParser::getTokenPrecedence() {
    if (isascii(this->currentToken))
        return -1;
    int tokenPrecedence = this->operatorPrecedence[this->currentToken];
    if (tokenPrecedence <= 0) return -1;
    return tokenPrecedence;
}

std::unique_ptr<PrototypeAST> IronParser::ParsePrototype() {
    if (this->currentToken != IronLexer::token_identifier)
        return ErrorHandlerP("Expected function name in prototype");

    std::string functionName = lexer.identifier;
    this->getNextToken();

    // COME BACK TO THIS
}

int main() {
    IronParser parser;
    parser.operatorPrecedence['<'] = 10;
    parser.operatorPrecedence['+'] = 20;
    parser.operatorPrecedence['-'] = 20;
    parser.operatorPrecedence['*'] = 40;
}