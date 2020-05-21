//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#include "SyntaxError.hpp"

SyntaxError::SyntaxError(rec<lexToke> token, const std::string& expected)
: reportErr(token, "expected '" + expected + "'")  { //output the syntax error
}

SyntaxError::~SyntaxError() {
    
}
