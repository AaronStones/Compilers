//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020


#include <sstream>
#include "semanticCheck.hpp"
#include "error/VariableError.hpp"
#include "error/ExprError.hpp"

std::string semanticAnalysis::valName(compType def) {
    if (compType::real == def){ //return what type of token the values is
        return "REAL";
    }
    if (compType::inte == def){
        return "INTEGER";
    }
    if (compType::boole == def){
        return "BOOLEAN";
    } 
    else {
        return "INVALID";
    }
}

void semanticAnalysis::logAssi(rec<lexToke> x, rec<lexToke> declaration, compType fsg, compType lsg) { //my own code
    std::string errMsg = valName(fsg) + " could not be assigned using " + valName(lsg) + " numericals"; //log an Assignment error
    logVar(x, declaration, errMsg);
}

void semanticAnalysis::logSem(rec<lexToke> x, const std::string& msg) {
    auto message = std::make_shared<reportErr>(x, msg);
    err.push_back(message); //add a Semnatic error to the error vector
}

void semanticAnalysis::logVar(rec<lexToke> x, rec<lexToke> declaration, const std::string& msg) {//my own code
    auto message = std::make_shared<VariableError>(x, declaration, msg);
    err.push_back(message); //add a varable error to the err vector
}

void semanticAnalysis::logExpr(rec<lexToke> x, const std::string& division, compType fsg, compType lsg) { 
    auto message = std::make_shared<ExprError>(x, division, valName(fsg), valName(lsg));
    err.push_back(message); //add an expression error to the err vector
}

void semanticAnalysis:: boolCheck(rec<lexToke> x, compType lsg, compType fsg){ //my own code
    if(lsg != compType::inv && fsg != compType::inv) {      //check that if an INTEGER is used it is being matched with another INTEGER (an example)     
        if(lsg == compType::inv) { 
            lsg = fsg; 
        }
        else if(fsg == compType::inv) { 
            fsg = lsg; 
        }
        else{
            logExpr(x, "boolean, mismatch", lsg, fsg); //log that a REAL has been used with an INTEGER and vice versa
        }
    }
    else {
        return;
    }  
}

void semanticAnalysis::assiCheck(rec<lexToke> x, rec<lexToke> lsg, compType fsg) { //my own code
    bool exists = varExists(lsg);
    if(exists != true) { //check if the variable exists or not
        std::string errorMessage =  lsg->getContains() + " is an undeclared data structure";
        logSem(lsg, errorMessage); //variable does not exist log an error
        return;
    }

    else {
        auto def = declVar(lsg);
        auto type = typeVar(lsg);

        if(type == compType::inv) {  //if type is invalid 
            type = fsg; 
        }
        if(fsg == compType::inv) {  //if type is still invalid
            fsg = type; 
        }
        if(type != fsg) { 
            logAssi(x, def, type, fsg); //log an assignment error
        }
        else{
            return;
        }

    }
}

void semanticAnalysis::varDecl(rec<lexToke> x, compType def) { //my own code
    bool exists = varExists(x); //true or false if variable already exists
    if (exists == false){
        vars[x->getContains()] = {def, x}; //define variable
    }
    else { //varibale already defined
        std::string message = "Already defined variable - " + x->getContains();
        logVar(x, declVar(x), message);
    }
}

bool semanticAnalysis::varExists(rec<lexToke> x) {
    auto value = vars.find(x->getContains()) != vars.end(); //try and located the variable
    return value;
}

compType semanticAnalysis::typeVar(rec<lexToke> x) {
    auto val = varExists(x) ? vars.at(x->getContains()).subDivision : compType::inv; 
    return val;
}

rec<lexToke> semanticAnalysis::declVar(rec<lexToke> var) { //my own code
    bool exists = varExists(var);
    if(exists == false) { //check if the variable exists
        return NULL; 
    }
    else {
        auto answer =  vars.at(var->getContains()).Location; //return the variable location
        return answer;
    }
}

compType semanticAnalysis::varCheck(rec<lexToke> x) { //my own code
    bool exists = varExists(x);

    if(exists == true) { //if the varibale exists return it
        return vars.at(x->getContains()).subDivision;
    }
    else {
        std::string message = "The variable '" + x->getContains() + "' is undefined, please first define it"; //log that the varibale is not defined
        logSem(x, message);
        return compType::inv;
    }
}

compType semanticAnalysis::valCheck(rec<lexToke> x) { //my own code
    int val = defineType(x); //get the type of the data being analysed
    switch(val){
        case 1:
            return compType::inte; //return thatt ype of data
        break;
        case 2: 
            return compType::boole;
        break;
        default:
            return compType::inv;
    }
    
}

int semanticAnalysis::defineType(rec<lexToke> x){ //my own code
    if(x->getContains() == lexToke::inte) { //check what type the variable is 
        return 1;
    }
    if(x->getContains() == lexToke::real) {
        return 2;
    }
    else{
        return 3;
    }
}

compType semanticAnalysis::exprCheck(rec<lexToke> x, compType lsg, compType fsg) { //tracking of expression errors
    if(lsg == compType::inv && fsg == compType::inv) {return compType::inv;}   
    if(lsg == compType::inv) { lsg = fsg; }
    if(fsg == compType::inv) { fsg = lsg; }
    if(lsg != fsg) {
        logExpr(x, "binary", lsg, fsg);
        return compType::inv;
    }
    return lsg;
}