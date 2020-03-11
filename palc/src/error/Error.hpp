//
//  Error.hpp - PAL Compiler's error interface
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include <iostream>
#include "../common.hpp"
#include "../Token.hpp"

/// The base representation of a PAL language error.
class Error {
protected:
    RC<Token>    token_;
    String          message_;
    
public:
    
    /// Creates an error that originates at [token], with [message].
    Error(RC<Token> token, const String& message);
    
    /// Error destructor
    virtual ~Error();
    
    virtual void print(std::ostream& out) const;
    
    // MARK: - straight getters
    
    const RC<Token> token() const { return token_; }
    const String& message() const { return message_; }
};

/// Comparison function to allow sorting errors at the end of the compiler
/// invocation.
int compare(const Error& lhs, const Error& rhs);

/// Comparison operators overrides, thin call wrappers to [compare()]
inline bool operator<(const Error& lhs, const Error& rhs) {
    return compare(lhs, rhs) == -1;
}

/// Comparison operators overrides, thin call wrappers to [compare()]
inline bool operator>(const Error& lhs, const Error& rhs) {
    return compare(lhs, rhs) == 1;
}

/// Print [error] to an output stream [out].
inline std::ostream& operator<<(std::ostream& out, const Error& error) {
    error.print(out);
    return out;
}

