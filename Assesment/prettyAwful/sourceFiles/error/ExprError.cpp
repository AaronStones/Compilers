//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#include <sstream>
#include "ExprError.hpp"

ExprError::ExprError(rec<lexToke> token, const std::string& type, const std::string& lhs, const std::string& rhs): reportErr(token, "") { //my own code
    std::string buffMsg;
    
    buffMsg = " invalid operands to " + type + " expression" + " ('" + lhs + "' and '" + rhs + "')"; //output the expression error
    msgV = buffMsg;
}
