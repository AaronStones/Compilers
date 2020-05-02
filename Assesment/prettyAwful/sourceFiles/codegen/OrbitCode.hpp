//
//  OrbitCode.hpp - Orbit Opcodes Enumerator
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once

// The orbit opcodes. We use the x macro because that's how the opcodes are
// defined in the OrbitVM source code.
#define OPCODE(x, _, __) CODE_##x,
    enum OrbitCode {
#include "orbit_opcodes.h"
    };
#undef OPCODE