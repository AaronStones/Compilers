//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#include "CodeGen.hpp"

#define OPCODE(x, _, __) #x,
static const std::string mnemonics[] = {
#include "orbit_opcodes.h"
};
#undef OPCODE

generation::generation()
    : ifD(0), loopD(0) {
}

generation::~generation() {}

void generation::startMod(std::ostream& out) const {
    out << build;
}

void generation::startProgram() {
    build.openFunction("main");
}

void generation::endProgram() {
    build.closeFunction();
}

void generation::startIf() {
    ifNum.push_back(ifD++);
}

void generation::closeIf() {
    build.function()->addSymbol(endifLabel());
    ifNum.pop_back();
}

std::string generation::stringMake(std::string var, std::string type){ //my own code 
    if (type == "if"){
        return var + std::to_string(ifNum.back());
    }
    else {
        return var + std::to_string(loopNum.back());
    }
}

std::string  generation::ifLabel() {//my own code 
    return stringMake("if_", "if");
}

std::string  generation::elseLabel() {//my own code 
    return stringMake("else_", "if");
}

std::string  generation::endifLabel() {//my own code 
    return stringMake("endif_", "if");
}

void generation::startLoop() {//my own code 
    loopNum.push_back(loopD++);
}

void generation::closeLoop() {//my own code 
    build.function()->addSymbol(endLoopLabel());
    loopNum.pop_back();
}

std::string  generation::loopLabel() {//my own code 
    return stringMake("loop_", "loop");

}

std::string  generation::endLoopLabel() {//my own code 
    return stringMake("endloop_", "loop");
}

void generation::homeFunc(const std::string & name) {
    build.function()->addLocal(name);
}

void generation::Number(OrbitCode code, double arg) {
    auto* instruction = build.function()->addInstruction(code);
    instruction->setOperand(build.addConstant(arg));
    build.function()->finishInstruction();
}

void generation::instructions(OrbitCode code, const std::string & arg, std::string operand){//my own code 
    auto* instruction = build.function()->addInstruction(code);
    if (operand == "getlocal"){
        instruction->setOperand(build.function()->getLocal(arg));
    }
    if (operand == "setoperand"){
        instruction->setOperand(arg);
    }
    if (operand == "addconstant"){
        instruction->setOperand(build.addConstant(arg));
    }
    build.function()->finishInstruction();
}

void generation::Variable(OrbitCode code, const std::string & arg) {//my own code 
    instructions(code, arg, "getlocal");
}

void generation::Leap(OrbitCode code, const std::string & label) {//my own code 
    instructions(code, label, "setoperand");

}

void generation::emitString(OrbitCode code, const std::string & str) {//my own code 
    instructions(code, str, "addconstant");
}

void generation::emit(OrbitCode code) {
    build.function()->addInstruction(code);
    build.function()->finishInstruction();
}

void generation::labels(const std::string & label) {
    build.function()->addSymbol(label);
}
