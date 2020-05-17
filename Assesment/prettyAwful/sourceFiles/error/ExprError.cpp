#include <sstream>
#include "ExprError.hpp"

ExprError::ExprError(rec<lexToke> token, const std::string& type, const std::string& lhs, const std::string& rhs): reportErr(token, "") {
    std::string buffMsg;
    
    buffMsg = " invalid operands to " + type + " expression" + " ('" + lhs + "' and '" + rhs + "')";
    msgV = buffMsg;
}
