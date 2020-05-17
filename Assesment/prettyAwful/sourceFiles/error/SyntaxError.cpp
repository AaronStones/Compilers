#include "SyntaxError.hpp"

SyntaxError::SyntaxError(rec<lexToke> token, const std::string& expected)
: reportErr(token, "expected '" + expected + "'")  {
}

SyntaxError::~SyntaxError() {
    
}
