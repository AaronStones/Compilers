//
//  ILBuilder.cpp
//  orbitasm
//
//  Created by Cesar Parent on 07/02/2017.
//  Copyright © 2017 cesarparent. All rights reserved.
//
//no changes have been made to these files as they contain orbatism (a concept I am unfamiliar with)
#include <algorithm>
#include <cmath>
#include "ILBuilder.hpp"

#define OPCODE(op, size, _) ILInstruction::OperandSize(size),
static ILInstruction::OperandSize operandSizes[] = {
#include "orbit_opcodes.h"
};
#undef OPCODE

// MARK: Module File Writer

std::ostream& operator<<(std::ostream& out, const ILBuilder& module) {
    
    ILWriter writer(out);
    
    writer.write(uint16_t(module.constants_.size()));
    for(auto& constant : module.constants_) {
        constant.pack(writer);
    }
    
    writer.write(uint16_t(module.globals_.size()));
    for(auto& global : module.globals_) {
        writer.write(ILWriter::OMFTag::VARIABLE);
        writer.write(global);
    }
    
    // No types atm
    writer.write(uint16_t(0));
    
    writer.write(uint16_t(module.functions_.size()));
    for(auto& function : module.functions_) {
        function.pack(writer);
    }
    
    return out;
}

void ILInstruction::pack(ILWriter& writer) const {
    writer.write(uint8_t(opcode_));
    
    switch(operandSize_) {
        case OperandSize::None:
            break;
            
        case OperandSize::Single:
            writer.write(uint8_t(operand_ & 0x00ff));
            break;
            
        case OperandSize::Double:
            writer.write(uint16_t(operand_));
            break;
    }
}

void ILFunction::pack(ILWriter& writer) const {
    writer.write(ILWriter::OMFTag::FUNCTION);
    
    writer.write(signature_);
    writer.write(uint8_t(arity_));
    writer.write(uint8_t(locals_.size() - arity_));
    writer.write(uint8_t(stackEffect_));
    
    writer.write(insertPoint_);
    for(auto& inst : byteCode_) {
        inst.pack(writer);
    }
}

void ILConstant::pack(ILWriter& writer) const {
    if(type == ILType::Number) {
        writer.write(numValue);
    } else {
        writer.write(stringValue);
    }
}


// MARK: - Semantics Analysis

#define OPCODE(op, _, effect) {CODE_##op, effect},
const mp<uint8_t, int8_t> ILInstruction::stackEffects = {
#include "orbit_opcodes.h"
};
#undef OPCODE


ILInstruction::ILInstruction(ILBuilder* builder, uint16_t offset, OrbitCode opcode)
: builder_(builder)
, effect_(0)
, offset_(offset)
, opcode_(opcode)
, complete_(false)
, operandSize_(operandSizes[opcode])
, operand_(0x0000) {
    effect_ = stackEffects.at(opcode);
    if(operandSize_ == OperandSize::None) {
        complete_ = true;
    }
}

uint8_t ILInstruction::getWidth() const {
    switch (operandSize_) {
        case OperandSize::None:
            return 1;
            break;
            
        case OperandSize::Single:
            return 2;
            break;
            
        case OperandSize::Double:
            return 3;
            break;
    }
    return 0;
}

void ILInstruction::setOperand(uint8_t operand) {
    if(operandSize_ != OperandSize::Single) {
        return;
    }
    complete_ = true;
    operand_ = operand;
}

void ILInstruction::setOperand(uint16_t operand) {
    if(operandSize_ != OperandSize::Double) {
        return;
    }
    complete_ = true;
    operand_ = operand;
}

void ILInstruction::setOperand(const std::string& label) {
    if(operandSize_ != OperandSize::Double) {
        return;
    }
    complete_ = false;
    label_ = label;
}

// MARK: - ILFunction implementation

void ILFunction::validate() {
    
    stackEffect_ = 0;
    int8_t stackPointer = 0;
    
    for(auto& inst : byteCode_) {
        stackPointer += inst.stackEffect();
        if(stackPointer > stackEffect_) {
            stackEffect_ = stackPointer;
        }
        
        if(inst.complete()) { continue; }
        int32_t target = getSymbol(inst.label());
        int32_t ip = inst.offset() +  inst.getWidth();
        inst.setOperand(uint16_t(std::abs(ip-target)));
    }
}

void ILFunction::finishInstruction() {
    if(byteCode_.size() > 0) {
        insertPoint_ += byteCode_.back().getWidth();
    }
}

ILInstruction* ILFunction::addInstruction(OrbitCode opcode) {
    byteCode_.push_back(ILInstruction(builder_, insertPoint_, opcode));
    return &byteCode_.back();
}

void ILFunction::addSymbol(const std::string& symbol) {
    if(symbols_.find(symbol) != symbols_.end()) {
        return;
    }
    symbols_[symbol] = insertPoint_;
}

uint16_t ILFunction::getSymbol(const std::string& symbol) {
    if(symbols_.find(symbol) == symbols_.end()) {
        return 0;
    }
    return symbols_[symbol];
}

void ILFunction::addParam(const std::string& param) {
    arity_ += 1;
    addLocal(param);
}

void ILFunction::addLocal(const std::string& local) {
    if(std::find(locals_.begin(), locals_.end(), local) != locals_.end()) {
        return;
    }
    if(locals_.size() > UINT8_MAX) {
        return;
    }
    locals_.push_back(local);
}

uint8_t ILFunction::getLocal(const std::string& local) {
    // builder_->semanticsError("unknown label \"" + symbol + "\"");
    auto it = std::find(locals_.begin(), locals_.end(), local);
    if(it == locals_.end()) {
        return 0;
    }
    return it - locals_.begin();
}

// MARK: - ILBuilder implementation

#define OPCODE(op, _, __) {#op, CODE_##op },
const mp<std::string, OrbitCode> ILBuilder::opCodes_ = {
#include "orbit_opcodes.h"
};
#undef OPCODE

OrbitCode ILBuilder::opcode(const std::string &mnemonic) {
    return opCodes_.at(mnemonic);
}


ILFunction* ILBuilder::openFunction(const std::string& signature) {
    if(currentFn_ != nullptr) {
        return currentFn_;
    }
    functions_.push_back(ILFunction(this, signature));
    return currentFn_ = &functions_.back();
}

void ILBuilder::closeFunction() {
    if(currentFn_ == nullptr) { return; }
    currentFn_->validate();
    currentFn_ = nullptr;
}

void ILBuilder::addGlobal(const std::string& global) {
    if(std::find(globals_.begin(), globals_.end(), global) != globals_.end()) {
        return;
    }
    if(globals_.size() > UINT16_MAX) {
        return;
    }
    globals_.push_back(global);
}

uint16_t ILBuilder::getGlobal(const std::string& global) {
    auto it = std::find(globals_.begin(), globals_.end(), global);
    if(it == globals_.end()) {
        return 0;
    }
    return it - globals_.begin();
}

uint16_t ILBuilder::addConstant(std::string string) {
    if(constants_.size() > UINT16_MAX) {
        return 0;
    }
    if(string.back() == '"') string.pop_back();
    if(string.front() == '"') string.erase(0, 1);
    
    constants_.push_back(ILConstant(string));
    return constants_.size() - 1;
}

uint16_t ILBuilder::addConstant(double number) {
    if(constants_.size() > UINT16_MAX) {
        return 0;
    }
    constants_.push_back(ILConstant(number));
    return constants_.size() - 1;
}
