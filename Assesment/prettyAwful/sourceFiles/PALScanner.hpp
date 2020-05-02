//
//  PALScanner.hpp - PAL Compiler's scanner interface
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include <iostream>
#include "common.hpp"
#include "Token.hpp"

class PALScanner {
    
    /// The number of the source file line currently being lexed.
    UInt64          line_;
    
    /// The index of the curent char in the current line.
    UInt64          idx_;
    
    /// The input stream used to read the source file.
    std::istream&   input_;
    
    /// The current line being parsed. We use this for better error signaling.
    String          currentLine_;
    
    /// A pointer to the current token.
    RC<Token>    currentToken_;
    
    /// The current character.
    char            currentChar_;
    
    /// States allowed in the FSM. Simpler and safer than ints.
    enum class State {
        Whitespace,
        Identifier,
        Integer,
        Real,
        Punctuation,
        EndOfFile,
        InvalidChar,
    };
    
public:
    
    /// The list of reserved keywords in PAL.
    static const Vector<String> keywords;
    
    /// Creates a scanner for the file pointed to by [input].
    PALScanner(std::istream& input);
    
    /// Scanner destructor.
    ~PALScanner();
    
    /// Moves the current character forward by one and returns it.
    void nextChar();
    
    /// Lexes the next token in the source and returns.
    const RC<Token> nextToken();
    
    // MARK: - straight getters.
    
    char currentChar() const { return currentChar_; }
    const RC<Token> currentToken() const { return currentToken_; }
    UInt64 line() const { return line_; }
    UInt64 column() const { return idx_ + 1; }
};
