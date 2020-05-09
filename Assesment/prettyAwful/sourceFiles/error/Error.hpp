//
//  Error.hpp - PAL Compiler's error interface
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include <iostream>
#include "../declVar.hpp"
#include "../tokenHandler.hpp"

/// The base representation of a PAL language error.
class Error {
protected:
    rec<Token>    token_;
    str          message_;
    
public:
    
    /// Creates an error that originates at [token], with [message].
    Error(rec<Token> token, const str& message);
    
    /// Error destructor
    virtual ~Error();
    
    virtual void print(std::ostream& out) const;
    
    // MARK: - straight getters
    
    const rec<Token> token() const { return token_; }
    const str& message() const { return message_; }
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

