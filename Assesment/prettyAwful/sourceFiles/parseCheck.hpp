#include <iostream>

#include "codegen/CodeGen.hpp"
#include "scanFile.hpp"
#include "error/error.hpp"
#include "semanticCheck.hpp"
#include "declVar.hpp"


#pragma once


class ParseAnalysis {
    
    scanFile& getTokens;
    bool rec_; 
    vec<rec<reportErr>> err;
    semanticAnalysis checkChar; 
    generateAnalysis gen;
    
    void beginProgram(), start(), variableDecl(), parseStatement(), parseStatement2(), parseAssign(), parseLoop(), parseIf(), parseInOut(), parseBool();
    compType parseExpr(), parseF(), parseT(), parseVal();
    vec<rec<lexToke>> parseList();
    bool check(const std::string& type) const;
    void assure(const std::string& type); 
    void synError(const std::string& expected);

    
public:
    ParseAnalysis(scanFile& scanner); ~ParseAnalysis();
    bool beginAnalysis();
    generateAnalysis generator() const { return gen; }
    bool isRecovering() const { return rec_; }
    const vec<rec<reportErr>>& errors() const { return err; }
};
