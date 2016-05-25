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

#include <lexer.h>

enum Token
{
    token_eof = -1,
    token_identifier = -2,
    token_extern = -3,

    // Setting up a package
    token_package = -4,
    token_requires = -5,

    // Defining functions
    // token_function = -4,

    // Variable assignment
    // token_size = -5,
    // token_type = -6,
    // token_identifier = -7,
    // token_assignment = -8,

    // Primitive data types
    token_boolean = -9,
    // token_small_signed_integer = -10,
};

static std::string identifier;
static bool boolean;

static int getToken()
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
        if (identifier == "boolean")
            return token_boolean;
        return token_identifier;
    }

    if (isdigit(lastChar) || lastChar == '.')
    {
        std::string number;
        do {
            number += lastChar;
            lastChat = getchar();
        } while (isdigit(lastChar) || lastChar == '.');
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