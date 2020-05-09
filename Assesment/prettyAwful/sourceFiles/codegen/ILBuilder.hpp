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
    
    static const Map<uint8_t, int8_t> stackEffects;
    
    ILInstruction(ILBuilder* builder, uint16_t offset, OrbitCode opcode);
    
    virtual ~ILInstruction() {}
    
    uint8_t getWidth() const;
    
    uint16_t offset() const { return offset_; }
    
    void setOperand(uint8_t operand);
    
    void setOperand(uint16_t operand);
    
    void setOperand(const String& operand);
    
    bool complete() const { return complete_; }
    
    int8_t stackEffect() const { return effect_; }
    
    const String& label() const { return label_; }
    
    void pack(ILWriter& writer) const;
    
private:

    ILBuilder*      builder_;
    int8_t          effect_;
    const uint16_t  offset_;
    const OrbitCode opcode_;
    bool            complete_;
    
    OperandSize     operandSize_;
    uint16_t        operand_;
    String          label_;
};

struct ILFunction {
    
    ILFunction(ILBuilder* builder, const String& signature)
        : builder_(builder), signature_(signature), arity_(0), insertPoint_(0) {}
    
    virtual ~ILFunction() {}
    
    ILInstruction* addInstruction(OrbitCode opcode);
    
    void finishInstruction();
    
    void addSymbol(const String& symbol);
    
    void addParam(const String& param);
    
    void addLocal(const String& local);
    
    uint16_t getSymbol(const String& symbol);
    
    uint8_t getLocal(const String& local);
    
    void validate();
    
    void pack(ILWriter& writer) const;
    
private:
    
    ILBuilder*              builder_;
    String                  signature_;
    int8_t                  stackEffect_;
    uint8_t                 arity_;
    uint16_t                insertPoint_;
    
    Map<String, uint16_t>   symbols_;
    Vector<String>          locals_;
    Vector<ILInstruction>   byteCode_;
};

enum class ILType {
    String,
    Number
};

struct ILConstant {
    
    ILType  type;
    String  stringValue;
    double  numValue;
    
    ILConstant(double number) : type(ILType::Number), stringValue(""), numValue(number) {}
    ILConstant(const String& str) : type(ILType::String), stringValue(str), numValue(0) {}
    
    void pack(ILWriter& writer) const;
};


class ILBuilder {
    
    ILFunction*         currentFn_;
    
    Vector<ILConstant>  constants_;
    Vector<ILFunction>  functions_;
    Vector<String>      globals_;
    
    static const Map<String, OrbitCode> opCodes_;
    
    friend std::ostream& operator<<(std::ostream& out, const ILBuilder& builder);
    
public:
    
    static OrbitCode opcode(const String& mnemonic);
    
    ILBuilder() : currentFn_(nullptr)  {}
    
    ~ILBuilder() {}
    
    ILFunction* openFunction(const String& signature);
    
    ILFunction* function() { return currentFn_; }
    
    void closeFunction();
    
    void addGlobal(const String& global);
    
    uint16_t getGlobal(const String& global);
    
    uint16_t addConstant(String string);
    
    uint16_t addConstant(double number);
    
};

std::ostream& operator<<(std::ostream& out, const ILBuilder& builder);
