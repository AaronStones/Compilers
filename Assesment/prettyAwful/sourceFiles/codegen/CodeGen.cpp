#include "CodeGen.hpp"
#define OPCODE(x, _, __) #x,
static const std::string mnemonics[] = { 
    #include "orbit_opcodes.h"
};
#undef OPCODE

generateAnalysis::generateAnalysis() : loopStatement(0), ifStatement(0) {}

generateAnalysis::~generateAnalysis() {}

std::string generateAnalysis::markerIf() {
    return createRet("if_" , ifS);
}

std::string generateAnalysis::markerElse() {
    return createRet("else_" , ifS);
}

std::string generateAnalysis::markerEndIf() {
    return createRet("endif" , ifS);
}

std::string generateAnalysis::markerloop() {
    return createRet("loop_" , ifS);
}

std::string generateAnalysis::markerEndLoop() {
    return createRet("endloop_" , ifS);
}

void generateAnalysis::ifBegin() {
    ifS[ifS.size()+1] = loopStatement++;
}

void generateAnalysis::ifFinish() {
    classBuild.function()->addSymbol(markerEndIf());
    ifS.pop_back();
}

void generateAnalysis::beginAnalysis() {
    classBuild.openFunction("main");
}

void generateAnalysis::loopBegin() {
    loop[loop.size()+1] = loopStatement++;
}

void generateAnalysis::loopFinish() {
    classBuild.function()->addSymbol(markerEndLoop());
    loop.pop_back();
}

void generateAnalysis::finishAnalysis() {
    classBuild.closeFunction();
}

std::string generateAnalysis::createRet(std::string start, vec<u_int64_t> value){
    start += std::to_string(value[value.size()]);
    return start;
}

void generateAnalysis::Construct(OrbitCode written, const std::string argsPass, std::string type) {
    auto* instruction = classBuild.function()->addInstruction(written);
    if (type != "" && argsPass != ""){
        if (type == "Var"){
            instruction->setOperand(classBuild.function()->getLocal(argsPass));
        }
        if (type == "Lab"){
            instruction->setOperand(argsPass);
        }
        if (type == "Str"){
            instruction->setOperand(classBuild.addConstant(argsPass));
        }
    }
    
    classBuild.function()->finishInstruction();
}

void generateAnalysis::divisional(const std::string& Variable) {
    classBuild.function()->addLocal(Variable);
}



void generateAnalysis::NumberConst(OrbitCode written, double argsPass) {
    auto* instruction = classBuild.function()->addInstruction(written);
    instruction->setOperand(classBuild.addConstant(argsPass));
    classBuild.function()->finishInstruction();
}

void generateAnalysis::VarConst(OrbitCode written, const std::string& argsPass) {
    Construct(written, argsPass, "Var");
}

void generateAnalysis::LabArg(OrbitCode written, const std::string& argsPass) {
    Construct(written, argsPass, "Lab");
}


void generateAnalysis::StrConst(OrbitCode written, const std::string& argsPass) {
    Construct(written, argsPass, "Str");
}

void generateAnalysis::withOArg(OrbitCode written) {
    Construct(written, "", "");
}

void generateAnalysis::modConstr(std::ostream& cmd) const {
    cmd << classBuild;
}

void generateAnalysis::LabConstr(const std::string& type) {
    classBuild.function()->addSymbol(type);
}





















