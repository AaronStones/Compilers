#pragma once
#include "Error.hpp"


class VariableError : public reportErr {
    
    rec<lexToke>    userDecl;
    
public:
    
    /// Creates an error related to the usage of the variable declared at
    /// [varDecl]. The error occurs at [token] and shows [message].
    VariableError(rec<lexToke> token, rec<lexToke> varDecl, const std::string& message);
    
    /// Error destructor
    virtual ~VariableError();
    
    virtual void print(std::ostream& out) const override;
};
