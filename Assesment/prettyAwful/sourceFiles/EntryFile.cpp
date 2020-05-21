//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>

#include "declVar.hpp"
#include "scanFile.hpp"
#include "parseCheck.hpp"
 
int main(int num, const char** arguments) {
    int usageError(const char** arguments);
    int exitComp(const std::string& message); //declare functions


    if(num != 2) {
        return usageError(arguments); //too many parameters for compiler 
    }
    
    std::ifstream inputFile{arguments[1]};
    if(inputFile.is_open()) {
        //file is open do nothing 
    }
    else {
        return exitComp("file '" + std::string(arguments[1]) + "' does not exist");
    }

    std::string outfile = "a.omf";
    
    // initilaise scanner & parser
    scanFile scanAna{inputFile};
    ParseAnalysis parseAna{scanAna};
    
    if(!parseAna.beginAnalysis()) {
        //begin compiling the code
        
        for(const auto& errList : parseAna.errors()) {
            std::cerr << *errList << std::endl;
        }
        switch (parseAna.errors().size()){
            case 0:
                std::cerr << "code compiled successfully!\n";
            break;
            case 1:
                std::cerr << parseAna.errors().size() << " error generated\n";
            break;
            default :
                std::cerr << parseAna.errors().size() << " errors generated\n";
            break;
        }
    } 
    else {
        std::ofstream out{outfile};
        if(!out.is_open()) {
            return exitComp("cannot open '" + outfile + "' for reading");
        }
        parseAna.generator().startMod(out);
    }
}


// Prints the usage message and returns the failure error code.
int usageError(const char** arguments) {
    std::cerr << arguments[0] << ": error: wrong number of arguments" << std::endl;
    std::cerr << "usage: " << arguments[0] << std::endl;
    return EXIT_FAILURE;
}

// Prints an error and returns the failure error code.
int exitComp(const std::string& message) {
    std::cerr << "error: " << message << std::endl;
    return EXIT_FAILURE;
}

