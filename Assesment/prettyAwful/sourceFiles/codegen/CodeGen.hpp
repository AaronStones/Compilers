//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#pragma once
#include <iostream>
#include "../declVar.hpp"
#include "ILWriter.hpp"
#include "ILBuilder.hpp"
#include "OrbitCode.hpp"


class generation {
    
    ILBuilder build;
    std::uint64_t ifD, loopD;
    vec<std::uint64_t > ifNum, loopNum;
    
public:
    generation();
    ~generation();
    
    void startProgram(), endProgram(), startIf(), closeIf(), startLoop(), closeLoop();
    std::string ifLabel(), elseLabel(), endifLabel(), loopLabel(), endLoopLabel();

    void startMod(std::ostream& out) const;
    std::string stringMake(std::string var, std::string type);
    void instructions(OrbitCode code, const std::string & arg, std::string operand);
    void homeFunc(const std::string& name), labels(const std::string& label);
    void Number(OrbitCode code, double arg);
    void Variable(OrbitCode code, const std::string& arg), Leap(OrbitCode code, const std::string& label);
    void emitString(OrbitCode code, const std::string& str), emit(OrbitCode code);
    
};
