//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020

#pragma once
#include <cstring>
#include <iostream>

class lexToke {

    std::string      def;
    std::string      contains;
    std::uint64_t    lineNumber;
    std::uint64_t    columnNumber;
    std::string      inputSource;
    
    
public:
    lexToke(const std::string& def, const std::string& contains, std::uint64_t lineNumber, std::uint64_t columnNumber);
    lexToke(const std::string& def, std::uint64_t lineNumber, std::uint64_t columnNumber);
    ~lexToke();

    static const std::string eof, inte, ident, invT, real, invC;


    const std::string& getSource() const { 
        return inputSource; 
    }
    
    void fileSource(const std::string& lineNum) { 
        inputSource = lineNum; 
    }

    std::uint64_t getLineNum() const { 
        return lineNumber; 
    }

    std::uint64_t getColNum() const { 
        return columnNumber; 
    }

    bool complete(const std::string& definition) const { 
        if (definition != def){
            return false;
        }
        else {
            return true;
        }
    }
    
    double doubleValue() const { 
        auto contain = contains.c_str();
        auto containment = std::atof(contain);
        return containment; 
    }

    const std::string& getContains() const { 
        return contains; 
    }

    const std::string& getDef() const { 
        return def; 
    }



};

std::ostream& operator<<(std::ostream& out, const lexToke& token);
