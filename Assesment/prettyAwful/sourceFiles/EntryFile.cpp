#include <fstream>
#include <iostream>
#include <cstdlib>

#include "declVar.hpp"
#include "scanFile.hpp"
#include "parseCheck.hpp"
 
int main(int num, const char** arguments) {


    int usageError(const char** arguments);
    int exitComp(const str& message); //declare functions


    if(num != 2) {
        return usageError(arguments); //too many parameters for compiler 
    }
    
    std::ifstream inputFile{arguments[1]};
    if(inputFile.is_open()) {
        //file is open do nothing 
    }
    else {
        return exitComp("file '" + str(arguments[1]) + "' does not exist");
    }

    str outfile = "a.omf";
    
    // initilaise scanner & parser
    PALScanner scanAna{inputFile};
    PALParser parseAna{scanAna};
    
    if(!parseAna.invoke()) {

        //begin compiling the code
        
        for(const auto& errList : parseAna.errors()) {
            std::cerr << *errList << std::endl;
        }

        if (parseAna.errors().size() > 1){
            std::cerr << parseAna.errors().size() << " errors generated" << std::endl;
        }
        else {
            std::cerr << parseAna.errors().size() << " error generated" << std::endl;
        }

    } 
    else {
        std::ofstream out{outfile};
        if(!out.is_open()) {
            return exitComp("cannot open '" + outfile + "' for reading");
        }
        parseAna.generator().writeModule(out);
    }
}


// Prints the usage message and returns the failure error code.
int usageError(const char** arguments) {
    std::cerr << arguments[0] << ": error: wrong number of arguments" << std::endl;
    std::cerr << "usage: " << arguments[0] << std::endl;
    return EXIT_FAILURE;
}

// Prints an error and returns the failure error code.
int exitComp(const str& message) {
    std::cerr << "error: " << message << std::endl;
    return EXIT_FAILURE;
}

