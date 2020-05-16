//
//  CodeGen.hpp - Orbit bytecode generator
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include <iostream>
#include "ILWriter.hpp"
#include "ILBuilder.hpp"
#include "OrbitCode.hpp"
#include "../declVar.hpp"



class CodeGen {
    
    // The Bytecode builder
    ILBuilder           builder_;
    
    // The next if statement ID. Used to generated unique labels.
    std::uint64_t              ifID_;
    
    // The next loop ID. Used to generated unique labels.
    std::uint64_t                 loopID_;
    
    // We maintain a stack of if and loop statements so that we can get back
    // to the enclosing IDs/labels when required.
    vec<std::uint64_t   >      ifStack_;
    vec<std::uint64_t   >      loopStack_;
    
public:
    
    // Creates a code generator outputing to [out].
    CodeGen();
    
    // CodeGen destructor.
    ~CodeGen();
    
    //
    void writeModule(std::ostream& out) const;
    
    // Emits OrbitIL preamble for the program (opens the main() function).
    void startProgram();
    
    // Emits OrbitIL end of program (closes the main() function).
    void endProgram();
    
    // Starts an if statement and pushes its id on the ifstack.
    void startIf();
    
    // Emits OrbitIL for the end of an if statement and pops it from the stack.
    void closeIf();
    
    // Returns the label for the if block of the topmost if statement.
    std::string ifLabel();
    
    // Returns the label for the else block of the topmost if statement.
    std::string elseLabel();
    
    // Returns the label for the end of the topmost if statement.
    std::string endifLabel();
    
    // Starts a loop and pushes its id on the loopstack.
    void startLoop();
    
    // Emits OrbitIL for the end of a loop and pops it from the stack.
    void closeLoop();
    
    // Returns the label for the start of the topmost loop.
    std::string loopLabel();
    
    // Returns the label for the end of the topmost loop.
    std::string endLoopLabel();
    
    // Emits OrbitIL declaring a local variable.
    void local(const std::string& name);
    
    // Emits an OrbitIL instruction with a number constant argument.
    void emitNum(OrbitCode code, double arg);
    
    // Emits an OrbitIL instruction with a local variable argument.
    void emitVar(OrbitCode code, const std::string& arg);
    
    // Emits an OrbitIL instruction with a jump label argument.
    void emitJump(OrbitCode code, const std::string& label);
    
    // Emits an OrbitIL instruction with a string constant argument.
    void emitString(OrbitCode code, const std::string& str);
    
    // Emits an OrbitIL instruction without argument.
    void emit(OrbitCode code);
    
    // Emits an OrbitIL label.
    void emitLabel(const std::string& label);
};
