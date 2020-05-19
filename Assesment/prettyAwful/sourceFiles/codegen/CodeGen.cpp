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
static const std::string mnemonics[] = {
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

std::string  CodeGen::ifLabel() {
    return std::string ("if_") + std::to_string(ifStack_.back());
}

std::string  CodeGen::elseLabel() {
    return std::string ("else_") + std::to_string(ifStack_.back());
}

std::string  CodeGen::endifLabel() {
    return std::string ("endif_") + std::to_string(ifStack_.back());
}

void CodeGen::startLoop() {
    loopStack_.push_back(loopID_++);
}

void CodeGen::closeLoop() {
    builder_.function()->addSymbol(endLoopLabel());
    loopStack_.pop_back();
}

std::string  CodeGen::loopLabel() {
    return std::string ("loop_") + std::to_string(loopStack_.back());
}

std::string  CodeGen::endLoopLabel() {
    return std::string ("endloop_") + std::to_string(loopStack_.back());
}

void CodeGen::local(const std::string & name) {
    builder_.function()->addLocal(name);
}

void CodeGen::emitNum(OrbitCode code, double arg) {
    auto* instruction = builder_.function()->addInstruction(code);
    instruction->setOperand(builder_.addConstant(arg));
    builder_.function()->finishInstruction();
}

void CodeGen::emitVar(OrbitCode code, const std::string & arg) {
    auto* instruction = builder_.function()->addInstruction(code);
    instruction->setOperand(builder_.function()->getLocal(arg));
    builder_.function()->finishInstruction();
}

void CodeGen::emitJump(OrbitCode code, const std::string & label) {
    auto* instruction = builder_.function()->addInstruction(code);
    instruction->setOperand(label);
    builder_.function()->finishInstruction();
}


void CodeGen::emitString(OrbitCode code, const std::string & str) {
    auto* instruction = builder_.function()->addInstruction(code);
    instruction->setOperand(builder_.addConstant(str));
    builder_.function()->finishInstruction();
}

void CodeGen::emit(OrbitCode code) {
    builder_.function()->addInstruction(code);
    builder_.function()->finishInstruction();
}

void CodeGen::emitLabel(const std::string & label) {
    builder_.function()->addSymbol(label);
}
