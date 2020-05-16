#include "tokenHandler.hpp"

const std::string lexToke::ident = "IDENTIFIER";
const std::string lexToke::real = "REAL";
const std::string lexToke::inte = "INTEGER";
const std::string lexToke::eof = "EOF";
const std::string lexToke::invT = "TOKEN INVALID";
const std::string lexToke::invC  = "CHARACTER INVALID";


lexToke::lexToke(const std::string& type, const std::string& value, std::uint64_t line, std::uint64_t col)
: type_(type)
, value_(value)
, line_(line)
, column_(col) {
    
}

lexToke::lexToke(const std::string& type, std::uint64_t line, std::uint64_t col)
: type_(type)
, line_(line)
, column_(col) {
    
}

lexToke::~lexToke() {}

std::ostream& operator<<(std::ostream& out, const lexToke& token) {
    out << token.sourceLine();
    for(std::uint64_t i = 0; i < token.column()-1; ++i) {
        out << " ";
    }
    out << "^";
    for(std::uint64_t i = 0; i < token.type().length()-1; ++i) {
        out << "~";
    }  
    return out;
}
