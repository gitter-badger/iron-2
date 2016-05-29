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

IronLexer::IronLexer() {}

int IronLexer::getToken()
{
    static int lastChar = ' ';

    while(isspace(lastChar))
    {
        lastChar = getchar();
    }

    if (isalpha(lastChar))
    {
        identifier = lastChar;
        while (isalnum((lastChar = getchar())))
            identifier += lastChar;
        if (identifier == "package")
            return token_package;
        if (identifier == "requires")
            return token_requires;
        if (identifier == "function")
            return token_function;
        if (identifier == "extern")
            return token_extern;
        if (identifier == "package")
            return token_package;
        if (identifier == "boolean")
            return token_boolean;
        return token_identifier;
    }

    if (isdigit(lastChar) || lastChar == '.')
    {
        std::string number;
        do {
            number += lastChar;
            lastChar = getchar();
        } while (isdigit(lastChar) || lastChar == '.');
        numericValue = strtod(number.c_str(), 0);
        return token_number;
    }

    if (lastChar == '%')
    {
        do
            lastChar = getchar();
        while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');
        if (lastChar != EOF)
            return getToken();
    }

    if (lastChar == EOF)
        return token_eof;

    int thisChar = lastChar;
    lastChar = getchar();
    return thisChar;
}