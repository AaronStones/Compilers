//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#pragma once
#include "Error.hpp"

class ExprError : public reportErr {
public:
    
    ExprError(rec<lexToke> token, const std::string& type, const std::string& lhs, const std::string& rhs);
    virtual ~ExprError() {}
};
