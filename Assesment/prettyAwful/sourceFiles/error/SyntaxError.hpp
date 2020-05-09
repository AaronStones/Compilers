//
//  SyntaxError.hpp - PAL Compiler's syntax error representation
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include "Error.hpp"

/// Representation of a syntax error
class SyntaxError : public Error {
public:
    
    /// Creates an error where [token] is not [expected].
    SyntaxError(rec<Token> token, const str& expected);
    
    /// Error destructor
    virtual ~SyntaxError();
};
