//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#pragma once
#include "Error.hpp"


class VariableError : public reportErr {
    
    rec<lexToke>    userDecl;
    
public:
    

    VariableError(rec<lexToke> token, rec<lexToke> varDecl, const std::string& message);
    
    virtual ~VariableError();
    
    virtual void print(std::ostream& out) const override;
};
