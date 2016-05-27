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

#include "parser.h"

using namespace std;

class ExpressionAST {
public:
    virtual ~ExpressionAST() {}
};

// Variable
class VariableAST : public ExpressionAST {
    std::string name;
public:
    VariableAST(const std::string $name) : name(name) {}
};

// Binary operator
class BinaryOperatorAST : public ExpressionAST {
    char op;
    std::unique_ptr<ExpressionAST> LHS, RHS;
public:
    BinaryOperatorAST(
        char op,
        std::unique_ptr<ExpressionAST> LHS,
        std::unique_ptr<ExpressionAST> RHS) :
        op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

// Function call
class FunctionCallAST : public ExpressionAST {
    std::string caller;
    std::vector<std::unique_ptr<ExpressionAST>> arguments;
public:
    FunctionCallAST(
        const std::string callee,
        std::vector<std::unique_ptr<ExpressionAST>> arguments) :
        callee(callee), arguments(std::move(arguments)) {}
};

// Function prototype
class PrototypeAST {
    std::string name;
    std::vector<std::string> arguments;
public:
    PrototypeAST(
        const std::string name,
        std::vector<std::string> arguments) :
        name(name), arguments(std::move(arguments)) {}
};

// The actual function definition itself
class FunctionAST {
    std::unique_ptr<PrototypeAST> prototype;
    std::unique_ptr<ExpressionAST> body;
public:
    FunctionAST(
        std::unique_ptr<PrototypeAST> prototype,
        std::unique_ptr<ExpressionAST> body) :
        prototype(std::move(prototype)), body(std::move(body)) {}
};

// Number
class NumberAST : public ExpressionAST {
    double value;
public:
    NumberAST(double value) : value(value) {}
};