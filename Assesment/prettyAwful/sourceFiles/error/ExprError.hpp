//
//  ExprError.hpp - PAL Compiler's binary expression mismatch error
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include "Error.hpp"

/// Representation of a syntax error
class ExprError : public Error {
public:
    
    /// Creates an error where [token] is not [expected].
    ExprError(rec<lexToke> token, const std::string& type,
              const std::string& lhs, const std::string& rhs);
    
    /// Error destructor
    virtual ~ExprError() {}
};
