//
//  ILWriter.hpp
//  orbitasm
//
//  Created by Cesar Parent on 08/02/2017.
//  Copyright © 2017 cesarparent. All rights reserved.
//
#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include "OrbitCode.hpp"

struct ILWriter {
    
    enum class OMFTag {
        VARIABLE    = 0x01,
        CLASS       = 0x02,
        FUNCTION    = 0x03,
        STRING      = 0x04,
        NUM         = 0x05,
    };
    
    void write(OMFTag data);
    void write(uint8_t data);
    void write(uint16_t data);
    void write(uint32_t data);
    void write(uint64_t data);
    void write(double data);
    void write(const std::string& data);
    
    ILWriter(std::ostream& out);
    
private:
    
    std::ostream& out_;
    
};
