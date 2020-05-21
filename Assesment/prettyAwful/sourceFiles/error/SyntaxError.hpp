//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#pragma once
#include "Error.hpp"

class SyntaxError : public reportErr {
public:
    
    SyntaxError(rec<lexToke> token, const std::string& expected);
    
    virtual ~SyntaxError();
};
