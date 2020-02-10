#ifndef Value_h
#define Value_h

#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

class Value {
public:
    virtual string toString() const = 0;
    virtual bool equals(const Value *v) const = 0;
    
    virtual bool requiresMemory() const = 0;
};

const auto ValueHash =
    [](const Value *v) { return hash<string>{}(v->toString()); };
const auto ValueEqual =
    [](const Value *u, const Value *v) { return u->equals(v); };

class ConstantValue : public Value {
    int val;
    
public:
    ConstantValue(int v);
    ~ConstantValue();
    
    string toString() const;
    bool equals(const Value *v) const;
    
    bool requiresMemory() const;
    
    int getValue();
};

class VariableValue : public Value {
    string name;
    int version;
    
public:
    VariableValue(string n);
    VariableValue(string n, int v);
    ~VariableValue();
    
    string toString() const;
    bool equals(const Value *v) const;
    
    bool requiresMemory() const;
    
    string getVariableName();
    int getVariableVersion();
};

class ComputedValue : public Value {
    int instrNumber;
    
public:
    ComputedValue(int num);
    ~ComputedValue();
    
    string toString() const;
    bool equals(const Value *v) const;
    
    bool requiresMemory() const;
    
    int getInstructionNumber();
};

class InheritedValue : public Value {
    Value *value;
    int blockNumber;
    
public:
    InheritedValue(Value *v, int num);
    ~InheritedValue();
    
    string toString() const;
    bool equals(const Value *v) const;
    
    bool requiresMemory() const;
    
    Value * getValue();
    int getOriginBlockNumber();
};

class InstructionAddressValue : public Value {
    int blockNumber;
    
public:
    InstructionAddressValue(int num);
    ~InstructionAddressValue();
    
    string toString() const;
    bool equals(const Value *v) const;
    
    bool requiresMemory() const;
    
    int getBlockNumber();
};

class RegisterValue : public Value {
    int registerNumber;
    
public:
    RegisterValue(int num);
    ~RegisterValue();
    
    string toString() const;
    bool equals(const Value *v) const;
    
    bool requiresMemory() const;
    
    int getRegisterNumber();
};

#endif /* Value_h */
