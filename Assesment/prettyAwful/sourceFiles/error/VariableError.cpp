//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#include "VariableError.hpp"

VariableError::VariableError(rec<lexToke> token, rec<lexToke> varDecl, const std::string& message): reportErr(token, message), userDecl(varDecl) {}

VariableError::~VariableError() {
    
}

void VariableError::print(std::ostream& out) const { //my own code

    reportErr::print(out);
    
    std::uint64_t line = userDecl->getLineNum();
    std::uint64_t col = userDecl->getColNum();
    bool complete = userDecl->complete(lexToke::eof);
    
    std::string val = "\n" + std::to_string(line) + ": " + std::to_string(col) 
    + ": note: variable '" + userDecl->getContains() 
    + "' was declared here";

    if (complete != true){
        val = val + "\n";
        out << val << *userDecl;
    } //output the variabel error
}
