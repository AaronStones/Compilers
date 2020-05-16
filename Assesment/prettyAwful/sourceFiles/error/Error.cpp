//
//  Error.cpp - PAL Compiler's error implementation
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include "Error.hpp"

Error::Error(rec<lexToke> token, const std::string& message)
: token_(token)
, message_(message) {
    
}

Error::~Error() {
    
}

/// Here we compare by line first (an error on line 1 will always come before
/// an error on line 10), and only compare columns for errors on the same line.
int compare(const Error& lhs, const Error& rhs) {
    if(lhs.token()->line() == rhs.token()->line()) {
        return lhs.token()->column() - rhs.token()->column();
    }
    return lhs.token()->line() - rhs.token()->line();
}

/// Basic error printing. No context printing at the moment, just line/column
/// and error message.
void Error::print(std::ostream& out) const {
    
    out << token()->line() << ":" << token()->column() << ": ";

    out << "error: ";

    out << message();

    if(!token()->is(lexToke::eof)) {
        out << std::endl << *token();
    }
}
