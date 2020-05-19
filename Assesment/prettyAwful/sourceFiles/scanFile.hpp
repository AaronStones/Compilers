#pragma once
#include <iostream>
#include "tokenHandler.hpp"
#include "declVar.hpp"


class scanFile {
    
    enum class compState {
        inte,
        real,
        punc,
        ident,
        white,
        eof,
        inv,
    };

    std::uint64_t    lineNum;
    std::uint64_t    lineIndex;
    std::istream& inFile;    
    std::string lineString;
    rec<lexToke> tokens;
    char character;
    
public:
    
    scanFile(std::istream& fileInput);
    ~scanFile();
    void getCharacter();
    int whiteSpace(char x);
    static const vec<std::string> hotKeys;
    int intCheck(char x);
    int realCheck(char x);
    const rec<lexToke> getNToken();
    

    char Character() const { return character; }
    std::uint64_t    currentColumn() const { return lineIndex + 1; }
    const rec<lexToke> getToken() const { return tokens; } 
    std::uint64_t    currentLine() const { return lineNum; }

};
