//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#include "tokenHandler.hpp"

const std::string lexToke::ident = "IDENTIFIER";
const std::string lexToke::real = "REAL";
const std::string lexToke::invT = "TOKEN INVALID";
const std::string lexToke::inte = "INTEGER";
const std::string lexToke::eof = "EOF";
const std::string lexToke::invC  = "CHARACTER INVALID"; //log types of token errors


lexToke::lexToke(const std::string& def_, const std::string& contains_, std::uint64_t lineNumber_, std::uint64_t columnNumber_)
: def(def_)
, contains(contains_)
, lineNumber(lineNumber_)
, columnNumber(columnNumber_) {}

lexToke::lexToke(const std::string& def_, std::uint64_t lineNumber_, std::uint64_t columnNumber_)
: def(def_)
, contains(def_)
, lineNumber(lineNumber_)
, columnNumber(columnNumber_) {}

lexToke::~lexToke() {}


std::ostream& operator<<(std::ostream& cmdOut, const lexToke& lex) {
    std::string output = lex.getSource();

    for(std::uint64_t i = 0; i < lex.getColNum()-1; ++i) {
        output += " ";
    }
    output += "*";
    return cmdOut << output; //output the operator handling errors
}


