//
//  ExprError.cpp - PAL Compiler's binary expression mismatch error
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include <sstream>
#include "ExprError.hpp"

ExprError::ExprError(rec<lexToke> token, const std::string& type,
                     const std::string& lhs, const std::string& rhs)
: Error(token, "") {
    std::stringstream buffer;
    buffer << "invalid operands to " << type << " expression";
    buffer << " ('" << lhs << "' and '" << rhs << "')";
    message_ = buffer.str();
}
