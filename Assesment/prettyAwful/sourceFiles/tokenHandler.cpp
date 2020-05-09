//
//  Token.cpp - PAL Compiler's token implementation
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include "tokenHandler.hpp"

const String Token::Identifier   = "identifier";
const String Token::Real         = "real";
const String Token::Integer      = "integer";
const String Token::EndOfFile    = "end of file";
const String Token::InvalidToken = "invalid token";
const String Token::InvalidChar  = "invalid character";

// MARK: - Constructor/Destructor implementation

Token::Token(const String& type, const String& value, UInt64 line, UInt64 col)
: type_(type)
, value_(value)
, line_(line)
, column_(col) {
    
}

Token::Token(const String& type, UInt64 line, UInt64 col)
: type_(type)
, value_(type)
, line_(line)
, column_(col) {
    
}

Token::~Token() {
    
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << token.sourceLine();
#ifdef ERROR_COLORFUL
    out << BOLDGREEN;
#endif
    //Then we display "squigglies" under the problematic token.
    for(UInt64 i = 0; i < token.column()-1; ++i) {
        out << " ";
    }
    out << "^";
    for(UInt64 i = 0; i < token.value().length()-1; ++i) {
        out << "~";
    }
#ifdef ERROR_COLORFUL
    out << RESET;
#endif
    
    return out;
}
