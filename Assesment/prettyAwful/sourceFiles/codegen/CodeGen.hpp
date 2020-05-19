#include "../declVar.hpp"
#include "ILWriter.hpp"
#include "ILBuilder.hpp"
#include "OrbitCode.hpp"

#include <iostream>

#pragma once

class generateAnalysis {
    
    ILBuilder classBuild;
    vec<std::uint64_t> loop, ifS;
    std::uint64_t loopStatement, ifStatement;
    
public:
    
    generateAnalysis(); ~generateAnalysis();
    
    std::string markerIf(), markerElse(), markerEndIf(), markerloop(), markerEndLoop();
    void ifBegin(), ifFinish(), beginAnalysis(), loopBegin(), loopFinish(), finishAnalysis();

    std::string createRet(std::string start, vec<u_int64_t> value);
    void Construct(OrbitCode written, const std::string argsPass, std::string type);

    void divisional(const std::string& Variable);
    void NumberConst(OrbitCode written, double argsPass), VarConst(OrbitCode written, const std::string& argsPass), LabArg(OrbitCode written, const std::string& argsPass), StrConst(OrbitCode written, const std::string& argsPass);
    
    void withOArg(OrbitCode written);
    void modConstr(std::ostream& cmd) const;
    void LabConstr(const std::string& type);
};
