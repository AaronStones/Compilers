#pragma once
#include <cstring>
#include <iostream>

class lexToke {

    std::string type_;
    std::string value_;
    std::uint64_t line_;
    std::uint64_t column_;
    std::string sourceLine_;
    
public:
    static const std::string ident;
    static const std::string real;
    static const std::string eof;
    static const std::string invT;
    static const std::string inte;
    static const std::string invC;

    lexToke(const std::string& type, const std::string& value, std::uint64_t line, std::uint64_t col);
    lexToke(const std::string& type, std::uint64_t line, std::uint64_t col);
    ~lexToke();
    
    void sourceLine(const std::string& line) { sourceLine_ = line; }
    bool is(const std::string& type) const { return type == type_; }
    double doubleValue() const { return std::atof(value_.c_str()); }
        
    std::uint64_t line() const { return line_; }
    std::uint64_t column() const { return column_; }
    const std::string& value() const { return value_; }
    const std::string& type() const { return type_; }
    const std::string& sourceLine() const { return sourceLine_; }
};

std::ostream& operator<<(std::ostream& out, const lexToke& token);
