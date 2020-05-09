//
//  Token.hpp - PAL Compiler's token interface
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include <cstring>
#include <iostream>
#include "declVar.hpp"

/// The representation of a PAL language token
class Token {

    String      type_;
    String      value_;
    UInt64      line_;
    UInt64      column_;
    String      sourceLine_;
    
public:
    
    // MARK: - Token types
    
    static const String Identifier;
    static const String Real;
    static const String Integer;
    static const String EndOfFile;
    static const String InvalidToken;
    static const String InvalidChar;
    
    // MARK: - Token instance methods
    
    /// Create a token with distinct [type] and string [value], starting at
    /// [line] and [col].
    Token(const String& type, const String& value, UInt64 line, UInt64 col);
    
    /// Create a token that has identical [type] and string value, starting at
    /// [line] and [col].
    Token(const String& type, UInt64 line, UInt64 col);
    
    /// Token destructor, not much to see here.
    ~Token();
    
    /// Sets the source line string
    void sourceLine(const String& line) { sourceLine_ = line; }
    
    /// Returns whether a token is of [type].
    bool is(const String& type) const { return type == type_; }
    
    /// Returns the double value of the token
    double doubleValue() const { return std::atof(value_.c_str()); }
    
    // MARK: - Straight getters
    
    UInt64 line() const { return line_; }
    UInt64 column() const { return column_; }
    const String& value() const { return value_; }
    const String& type() const { return type_; }
    const String& sourceLine() const { return sourceLine_; }
};

std::ostream& operator<<(std::ostream& out, const Token& token);
