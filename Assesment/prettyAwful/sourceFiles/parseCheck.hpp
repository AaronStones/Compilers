#include <iostream>

#include "codegen/CodeGen.hpp"
#include "scanFile.hpp"
#include "error/Error.hpp"
#include "semanticCheck.hpp"
#include "declVar.hpp"


#pragma once


class ParseAnalysis {
    
    scanFile& scanner_;

    bool rec_; 
    vec<rec<Error>> errors_;
    
    semanticAnalysis semantics_; 
    CodeGen codegen_;
    
    void beginProgram(); //begin program <program>
    void variableDecl(); //variable declaration

    void parseStatement();//<statement> checker
    void parseStatement2(); 

    void parseAssign();//assignment parser

    void parseLoop(); //syntax a loop
    void parseIf(); //if statement
    void parseInOut(); //input and output statement 
    void parseBool(); //boolean parser

    compType parseExpr(); //Expression statement parser
    compType parseF(); //factor expression parser
    compType parseT(); //term parser
    compType parseVal(); //value parser

    vec<rec<lexToke>> parseList(); //list parser


    bool check(const std::string& type) const; //make sure the token passed is of the same type
    void assure(const std::string& type); //check the token is of type and absorbs it, if not sends compiler into recovery

    void synError(const std::string& expected); //display any syntax errors found

    
public:
    ParseAnalysis(scanFile& scanner);
    ~ParseAnalysis();
    bool invoke();
    CodeGen generator() const { return codegen_; }
    bool isRecovering() const { return rec_; }
    const vec<rec<Error>>& errors() const { return errors_; }
};
