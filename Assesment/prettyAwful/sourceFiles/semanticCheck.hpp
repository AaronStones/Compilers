//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#include "tokenHandler.hpp"
#include "error/Error.hpp"
#include "declVar.hpp"
#pragma once

enum class compType {
    inte,
    real,
    boole,
    inv,
};

class semanticAnalysis {
   
    struct decl {
        compType subDivision;
        rec<lexToke> Location;
    };
    
    std::string valName(compType sub);
    void logAssi(rec<lexToke> x, rec<lexToke> declaration, compType fsg, compType lsg);
    void logExpr(rec<lexToke> x, const std::string& division, compType fsg, compType lsg);
    void logSem(rec<lexToke> x, const std::string& msg);
    void logVar(rec<lexToke> x, rec<lexToke> declaration, const std::string& msg);

    mp<std::string, decl> vars;
    vec<rec<reportErr>>& err;

    
public:
    
    semanticAnalysis(vec<rec<reportErr>>& errors) : err(errors) {} ~semanticAnalysis() {}

    void boolCheck(rec<lexToke> x, compType lsg, compType fsg), assiCheck(rec<lexToke> x, rec<lexToke> lsg, compType fsg);
    void varDecl(rec<lexToke> x, compType def);
    
    bool varExists(rec<lexToke> x);

    compType typeVar(rec<lexToke> x), valCheck(rec<lexToke> x), varCheck(rec<lexToke> x);
    compType exprCheck(rec<lexToke> x, compType lsg, compType fsg);
    int defineType(rec<lexToke> x);
    rec<lexToke> declVar(rec<lexToke> var);

};
