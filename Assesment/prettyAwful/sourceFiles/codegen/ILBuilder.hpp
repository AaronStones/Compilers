#pragma once
#include <cstdint>
#include "OrbitCode.hpp"
#include "ILWriter.hpp"
#include "../declVar.hpp"


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
    
    void setOperand(const std::string& operand);
    
    bool complete() const { return complete_; }
    
    int8_t stackEffect() const { return effect_; }
    
    const std::string& label() const { return label_; }
    
    void pack(ILWriter& writer) const;
    
private:

    ILBuilder*      builder_;
    int8_t          effect_;
    const uint16_t  offset_;
    const OrbitCode opcode_;
    bool            complete_;
    
    OperandSize     operandSize_;
    uint16_t        operand_;
    std::string          label_;
};

struct ILFunction {
    
    ILFunction(ILBuilder* builder, const std::string& signature)
        : builder_(builder), signature_(signature), arity_(0), insertPoint_(0) {}
    
    virtual ~ILFunction() {}
    
    ILInstruction* addInstruction(OrbitCode opcode);
    
    void finishInstruction();
    
    void addSymbol(const std::string& symbol);
    
    void addParam(const std::string& param);
    
    void addLocal(const std::string& local);
    
    uint16_t getSymbol(const std::string& symbol);
    
    uint8_t getLocal(const std::string& local);
    
    void validate();
    
    void pack(ILWriter& writer) const;
    
private:
    
    ILBuilder*              builder_;
    std::string                  signature_;
    int8_t                  stackEffect_;
    uint8_t                 arity_;
    uint16_t                insertPoint_;
    
    mp<std::string, uint16_t>   symbols_;
    vec<std::string>          locals_;
    vec<ILInstruction>   byteCode_;
};

enum class ILType {
    String,
    Number
};

struct ILConstant {
    
    ILType  type;
    std::string  stringValue;
    double  numValue;
    
    ILConstant(double number) : type(ILType::Number), stringValue(""), numValue(number) {}
    ILConstant(const std::string& str) : type(ILType::String), stringValue(str), numValue(0) {}
    
    void pack(ILWriter& writer) const;
};


class ILBuilder {
    
    ILFunction*         currentFn_;
    
    vec<ILConstant>  constants_;
    vec<ILFunction>  functions_;
    vec<std::string>      globals_;
    
    static const mp<std::string, OrbitCode> opCodes_;
    
    friend std::ostream& operator<<(std::ostream& out, const ILBuilder& builder);
    
public:
    
    static OrbitCode opcode(const std::string& mnemonic);
    
    ILBuilder() : currentFn_(nullptr)  {}
    
    ~ILBuilder() {}
    
    ILFunction* openFunction(const std::string& signature);
    
    ILFunction* function() { return currentFn_; }
    
    void closeFunction();
    
    void addGlobal(const std::string& global);
    
    uint16_t getGlobal(const std::string& global);
    
    uint16_t addConstant(std::string string);
    
    uint16_t addConstant(double number);
    
};

std::ostream& operator<<(std::ostream& out, const ILBuilder& builder);
