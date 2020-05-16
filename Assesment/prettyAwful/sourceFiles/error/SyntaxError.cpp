//
//  SyntaxError.hpp - PAL Compiler's syntax error representation
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include "SyntaxError.hpp"

SyntaxError::SyntaxError(rec<lexToke> token, const std::string& expected)
: Error(token, "expected '" + expected + "'")  {
}

SyntaxError::~SyntaxError() {
    
}
