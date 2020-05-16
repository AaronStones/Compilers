#include <algorithm>
#include "scanFile.hpp"
#include <sstream>
#include <cctype>


scanFile::~scanFile() {
    
}

scanFile::scanFile(std::istream& fileInput)
: lineNum(1), lineIndex(0), inFile(fileInput), tokens(nullptr), character(' ') {}



const vec<std::string> scanFile::hotKeys = {
	"PROGRAM", "IN", "WITH", "END", "AS", "INTEGER", "REAL", "REPEAT","UNTIL",
    "ENDIF", "IF", "ELSE", "THEN", "ENDLOOP", "OUTPUT", "INPUT"
};
void scanFile::getCharacter() {
    if(character != 25) { 
        
        lineIndex++;

        if(lineString.size() > lineIndex) {
            character = lineString[lineIndex];
            return;
        }
        
        if(inFile.eof() != true) {
            std::getline(inFile, lineString);
            lineString = lineString + "\n";
            lineNum++;
            lineIndex = 0;
            character = lineString.front();

        }
        else{
            character = 25;
            return;
        }
    }
}

const rec<lexToke> scanFile::getNToken() {
    int Line;
    int Column;
    rec<lexToke> errorToken;
    std::stringstream buffer;
    compState state = compState::white;

    
    while(!errorToken) {
        switch(state) {
            case compState::white:
                if(!std::isspace(Character()) == false) {
                    state = compState::white;
                } 
                else {
                    Line = currentLine();
                    Column = currentColumn(); 
                    switch (whiteSpace(Character())){
                        case 1:
                            state = compState::ident;
                            break;
                        case 2: 
                            state = compState::inte;
                            break;
                        case 3: 
                            state = compState::punc;
                            break;
                        case 4:
                            state = compState::eof;
                            break;
                        case 5: 
                            state = compState::inv;
                    }
                }
            break;
            
            case compState::ident:

                if(!std::isalnum(Character())) {
                    if(std::find(hotKeys.begin(), hotKeys.end(), buffer.str()) != hotKeys.end()) {
                        errorToken = std::make_shared<lexToke>(buffer.str(), Line, Column);
                    }
                    else {
                        errorToken = std::make_shared<lexToke>(lexToke::ident, buffer.str(), Line, Column);
                    }
                }
                else {
                    state = compState::ident;
                }            
            break;
                
            case compState::inte:
                switch(intCheck(Character())){
                    case 1:
                        state = compState::real;
                    break;
                    case 2:
                        state = compState::inte;
                    break;
                    case 3:
                        errorToken = std::make_shared<lexToke>(lexToke::inte, buffer.str(), Line, Column);
                    break;
                }
            break;

            case compState::real:
                if (realCheck(Character()) < 1){
                    errorToken = std::make_shared<lexToke>(lexToke::real, buffer.str(), Line, Column);
                }
                else {
                    state = compState::real;
                }
            break;
            case compState::eof:
                errorToken = std::make_shared<lexToke>(lexToke::eof, Line, Column);
            break;

            case compState::inv:
                errorToken = std::make_shared<lexToke>(lexToke::invT, buffer.str(), Line, Column);
                break;
            
                
            case compState::punc:
                errorToken = std::make_shared<lexToke>(buffer.str(), Line, Column);
            break;
        }

        if(!errorToken && state != compState::white) {
            buffer << Character();
        }
                
        if (!errorToken){
            getCharacter();
        }
    }
    errorToken->sourceLine(lineString);
    return (tokens = errorToken);
}

int scanFile::whiteSpace(char x) {
    if(std::isalpha(x)) {
        return 1;
    }
    else if(std::isdigit(x)) {
        return 2;
    }
    else if(std::string("+-*/(),=<>").find(x) != std::string::npos) {
        return 3;
    }
    else if(x == 25) {
        return 4;
    }
    else {
        return 5;
    }
}

int scanFile::intCheck(char x) {
    if(x == '.') {
        return 1;
    }
    if(std::isdigit(x) == true) {
        return 2;
    }
    
    else {
        return 3;
    }
}

int scanFile::realCheck(char x) {
    if(std::isdigit(x)) {
        return 1;
    }
    else {
        return 0;
    }
}

