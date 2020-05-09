//
//  ILBuilder.hpp
//  orbitasm
//
//  Created by Cesar Parent on 07/02/2017.
//  Copyright © 2017 cesarparent. All rights reserved.
//
#pragma once
#include <cstdint>
#include "../declVar.hpp"
#include "OrbitCode.hpp"
#include "ILWriter.hpp"

struct ILInstruction;
struct ILFunction;
struct ILConstant;
class ILBuilder;

struct ILInstruction {
    
    enum class OperandSize {
        None    = 0,
        Single  = 1,
        Double  = 2,
    };
    
    static const mp<uint8_t, int8_t> stackEffects;
    
    ILInstruction(ILBuilder* builder, uint16_t offset, OrbitCode opcode);
    
    virtual ~ILInstruction() {}
    
    uint8_t getWidth() const;
    
    uint16_t offset() const { return offset_; }
    
    void setOperand(uint8_t operand);
    
    void setOperand(uint16_t operand);
    
    void setOperand(const str& operand);
    
    bool complete() const { return complete_; }
    
    int8_t stackEffect() const { return effect_; }
    
    const str& label() const { return label_; }
    
    void pack(ILWriter& writer) const;
    
private:

    ILBuilder*      builder_;
    int8_t          effect_;
    const uint16_t  offset_;
    const OrbitCode opcode_;
    bool            complete_;
    
    OperandSize     operandSize_;
    uint16_t        operand_;
    str          label_;
};

struct ILFunction {
    
    ILFunction(ILBuilder* builder, const str& signature)
        : builder_(builder), signature_(signature), arity_(0), insertPoint_(0) {}
    
    virtual ~ILFunction() {}
    
    ILInstruction* addInstruction(OrbitCode opcode);
    
    void finishInstruction();
    
    void addSymbol(const str& symbol);
    
    void addParam(const str& param);
    
    void addLocal(const str& local);
    
    uint16_t getSymbol(const str& symbol);
    
    uint8_t getLocal(const str& local);
    
    void validate();
    
    void pack(ILWriter& writer) const;
    
private:
    
    ILBuilder*              builder_;
    str                  signature_;
    int8_t                  stackEffect_;
    uint8_t                 arity_;
    uint16_t                insertPoint_;
    
    mp<str, uint16_t>   symbols_;
    vec<str>          locals_;
    vec<ILInstruction>   byteCode_;
};

enum class ILType {
    String,
    Number
};

struct ILConstant {
    
    ILType  type;
    str  stringValue;
    double  numValue;
    
    ILConstant(double number) : type(ILType::Number), stringValue(""), numValue(number) {}
    ILConstant(const str& str) : type(ILType::String), stringValue(str), numValue(0) {}
    
    void pack(ILWriter& writer) const;
};


class ILBuilder {
    
    ILFunction*         currentFn_;
    
    vec<ILConstant>  constants_;
    vec<ILFunction>  functions_;
    vec<str>      globals_;
    
    static const mp<str, OrbitCode> opCodes_;
    
    friend std::ostream& operator<<(std::ostream& out, const ILBuilder& builder);
    
public:
    
    static OrbitCode opcode(const str& mnemonic);
    
    ILBuilder() : currentFn_(nullptr)  {}
    
    ~ILBuilder() {}
    
    ILFunction* openFunction(const str& signature);
    
    ILFunction* function() { return currentFn_; }
    
    void closeFunction();
    
    void addGlobal(const str& global);
    
    uint16_t getGlobal(const str& global);
    
    uint16_t addConstant(str string);
    
    uint16_t addConstant(double number);
    
};

std::ostream& operator<<(std::ostream& out, const ILBuilder& builder);
