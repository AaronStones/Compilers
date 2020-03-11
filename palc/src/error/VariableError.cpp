//
//  PALConflictError.cpp - PAL Compiler's variable-related error type
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include "VariableError.hpp"

VariableError::VariableError(RC<Token> token, RC<Token> varDecl, const String& message)
: Error(token, message)
, declaration_(varDecl) {
}

VariableError::~VariableError() {
    
}

void VariableError::print(std::ostream& out) const {
    Error::print(out);
    
    auto where = declaration_;
    
    // Once we've printed the error message, print the declaration context.
    out << std::endl;
#ifdef ERROR_COLORFUL
    out << RESET;
#endif
    out << where->line() << ":" << where->column() << ": ";
#ifdef ERROR_COLORFUL
    out << BOLDMAGENTA;
#endif
    out << "note: ";
#ifdef ERROR_COLORFUL
    out << RESET;
#endif
    out << "variable '" << where->value() << "' was declared here";
    
    if(!where->is(Token::EndOfFile)) {
        out << std::endl << *where;
    }
}
