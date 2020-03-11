//
//  CodeGen.cpp - Orbit IL code generator
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include "CodeGen.hpp"


// And here we have the code -> mnemonic conversion table. This uses the
// x-macro too.
#define OPCODE(x, _, __) #x,
static const String mnemonics[] = {
#include "orbit_opcodes.h"
};
#undef OPCODE

CodeGen::CodeGen()
    : ifID_(0), loopID_(0) {
}

CodeGen::~CodeGen() {
    
}

void CodeGen::writeModule(std::ostream& out) const {
    out << builder_;
}

void CodeGen::startProgram() {
    builder_.openFunction("main");
}

void CodeGen::endProgram() {
    builder_.closeFunction();
}

void CodeGen::startIf() {
    ifStack_.push_back(ifID_++);
}

void CodeGen::closeIf() {
    builder_.function()->addSymbol(endifLabel());
    ifStack_.pop_back();
}

String CodeGen::ifLabel() {
    return String("if_") + std::to_string(ifStack_.back());
}

String CodeGen::elseLabel() {
    return String("else_") + std::to_string(ifStack_.back());
}

String CodeGen::endifLabel() {
    return String("endif_") + std::to_string(ifStack_.back());
}

void CodeGen::startLoop() {
    loopStack_.push_back(loopID_++);
}

void CodeGen::closeLoop() {
    builder_.function()->addSymbol(endLoopLabel());
    loopStack_.pop_back();
}

String CodeGen::loopLabel() {
    return String("loop_") + std::to_string(loopStack_.back());
}

String CodeGen::endLoopLabel() {
    return String("endloop_") + std::to_string(loopStack_.back());
}

void CodeGen::local(const String& name) {
    builder_.function()->addLocal(name);
}

void CodeGen::emitNum(OrbitCode code, double arg) {
    auto* instruction = builder_.function()->addInstruction(code);
    instruction->setOperand(builder_.addConstant(arg));
    builder_.function()->finishInstruction();
}

void CodeGen::emitVar(OrbitCode code, const String& arg) {
    auto* instruction = builder_.function()->addInstruction(code);
    instruction->setOperand(builder_.function()->getLocal(arg));
    builder_.function()->finishInstruction();
}

void CodeGen::emitJump(OrbitCode code, const String& label) {
    auto* instruction = builder_.function()->addInstruction(code);
    instruction->setOperand(label);
    builder_.function()->finishInstruction();
}


void CodeGen::emitString(OrbitCode code, const String& str) {
    auto* instruction = builder_.function()->addInstruction(code);
    instruction->setOperand(builder_.addConstant(str));
    builder_.function()->finishInstruction();
}

void CodeGen::emit(OrbitCode code) {
    builder_.function()->addInstruction(code);
    builder_.function()->finishInstruction();
}

void CodeGen::emitLabel(const String& label) {
    builder_.function()->addSymbol(label);
}
