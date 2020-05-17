#pragma once
#include "Error.hpp"

class SyntaxError : public reportErr {
public:
    
    SyntaxError(rec<lexToke> token, const std::string& expected);
    
    virtual ~SyntaxError();
};
