//
//  VariableError.hpp - PAL Compiler's variable-related error type
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include "Error.hpp"

/// Representation of a syntax error
class VariableError : public Error {
    
    /// We store where the variable is declared. This allows us to print a hint
    /// to the user with the actual declaration.
    RC<Token>    declaration_;
    
public:
    
    /// Creates an error related to the usage of the variable declared at
    /// [varDecl]. The error occurs at [token] and shows [message].
    VariableError(RC<Token> token, RC<Token> varDecl, const String& message);
    
    /// Error destructor
    virtual ~VariableError();
    
    virtual void print(std::ostream& out) const override;
};
