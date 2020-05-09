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

    str      type_;
    str      value_;
    UInt64      line_;
    UInt64      column_;
    str      sourceLine_;
    
public:
    
    // MARK: - Token types
    
    static const str Identifier;
    static const str Real;
    static const str Integer;
    static const str EndOfFile;
    static const str InvalidToken;
    static const str InvalidChar;
    
    // MARK: - Token instance methods
    
    /// Create a token with distinct [type] and string [value], starting at
    /// [line] and [col].
    Token(const str& type, const str& value, UInt64 line, UInt64 col);
    
    /// Create a token that has identical [type] and string value, starting at
    /// [line] and [col].
    Token(const str& type, UInt64 line, UInt64 col);
    
    /// Token destructor, not much to see here.
    ~Token();
    
    /// Sets the source line string
    void sourceLine(const str& line) { sourceLine_ = line; }
    
    /// Returns whether a token is of [type].
    bool is(const str& type) const { return type == type_; }
    
    /// Returns the double value of the token
    double doubleValue() const { return std::atof(value_.c_str()); }
    
    // MARK: - Straight getters
    
    UInt64 line() const { return line_; }
    UInt64 column() const { return column_; }
    const str& value() const { return value_; }
    const str& type() const { return type_; }
    const str& sourceLine() const { return sourceLine_; }
};

std::ostream& operator<<(std::ostream& out, const Token& token);
