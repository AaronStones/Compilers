#include <sstream>
#include "semanticCheck.hpp"
#include "error/VariableError.hpp"
#include "error/ExprError.hpp"

std::string semanticAnalysis::valName(compType def) {
    if (def == compType::boole){
        return "bool";
    }
    if (def == compType::inte){
        return "int";
    }
    if (def == compType::real){
        return "real";
    }
    else{
        return "Invalid";
    }
}

void semanticAnalysis::logAssi(rec<lexToke> x, rec<lexToke> declaration, compType fsg, compType lsg) {
    std::string errMsg = valName(fsg) + " could not be assigned using '" + valName(lsg) + "' numericals";
    logVar(x, declaration, errMsg);
}

void semanticAnalysis::logSem(rec<lexToke> x, const std::string& msg) {
    auto message = std::make_shared<Error>(x, msg);
    err.push_back(message);
}

void semanticAnalysis::logVar(rec<lexToke> x, rec<lexToke> declaration, const std::string& msg) {
    auto message = std::make_shared<VariableError>(x, declaration, msg);
    err.push_back(message);
}

void semanticAnalysis::logExpr(rec<lexToke> x, const std::string& division, compType fsg, compType lsg) {
    auto message = std::make_shared<ExprError>(x, division, valName(fsg), valName(lsg));
    err.push_back(message);
}

void semanticAnalysis:: boolCheck(rec<lexToke> x, compType lsg, compType fsg){
    if(lsg != compType::inv && fsg != compType::inv) {         
        if(lsg == compType::inv) { 
            lsg = fsg; 
        }
        else if(fsg == compType::inv) { 
            fsg = lsg; 
        }
        else{
            logExpr(x, "boolean, mismatch", lsg, fsg);
        }
    }
    else {
        return;
    }  
}

void semanticAnalysis::assiCheck(rec<lexToke> x, rec<lexToke> lsg, compType fsg) {
    bool exists = varExists(lsg);
    if(exists != true) {
        std::string errorMessage =  lsg->value() + " is an undeclared data structure";
        logSem(lsg, errorMessage);
        return;
    }

    else {
        auto def = declVar(lsg);
        auto type = typeVar(lsg);

        if(type == compType::inv) { 
            type = fsg; 
        }
        if(fsg == compType::inv) { 
            fsg = type; 
        }
        if(type != fsg) { 
            logAssi(x, def, type, fsg);
        }
        else{
            return;
        }

    }
}

void semanticAnalysis::varDecl(rec<lexToke> x, compType def) {
    bool exists = varExists(x); //true or false if variable already exists
    if (exists == false){
        vars[x->value()] = {def, x}; //define variable
    }
    else { //varibale already defined
    std::string message = "Already defined variable - " + x->value();
        logVar(x, declVar(x), message);
    }
}

bool semanticAnalysis::varExists(rec<lexToke> x) {
    auto value = vars.find(x->value()) != vars.end();
    return value;
}

compType semanticAnalysis::typeVar(rec<lexToke> x) {
    auto val = varExists(x) ? vars.at(x->value()).subDivision : compType::inv;
    return val;
}

rec<lexToke> semanticAnalysis::declVar(rec<lexToke> var) {
    bool exists = varExists(var);
    if(exists == false) { 
        return NULL; 
    }
    else {
        auto answer =  vars.at(var->value()).Location;
        return answer;
    }
}

compType semanticAnalysis::varCheck(rec<lexToke> x) {
    bool exists = varExists(x);

    if(exists == true) {
        return vars.at(x->value()).subDivision;
    }
    else {
        std::string message = "The variable '" + x->value() + "' is undefined, please first define it";
        logSem(x, message);
        return compType::inv;
    }
}

compType semanticAnalysis::valCheck(rec<lexToke> x) {
    int val = defineType(x);
    switch(val){
        case 1:
            return compType::inte;
        break;
        case 2: 
            return compType::boole;
        break;
        default:
            return compType::inv;
    }
    
}

int semanticAnalysis::defineType(rec<lexToke> x){
    if(x->value() == lexToke::inte) {
        return 1;
    }
    if(x->value() == lexToke::real) {
        return 2;
    }
    else{
        return 3;
    }
}

compType semanticAnalysis::exprCheck(rec<lexToke> x, compType lsg, compType fsg) {
    if(lsg == compType::inv && fsg == compType::inv) { 
        return compType::inv; 
    }
    
    if(lsg == fsg) {
        return lsg;
    }
    else {
        logExpr(x, "binary", lsg, fsg);
        return compType::inv;
    }
    
}






