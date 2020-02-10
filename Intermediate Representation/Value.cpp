#include "Value.h"

ConstantValue::ConstantValue(int v) {
    val = v;
}

ConstantValue::~ConstantValue() {}

string ConstantValue::toString() const {
    return "#" + to_string(val);
}

bool ConstantValue::equals(const Value *v) const {
    if (auto *c = dynamic_cast<const ConstantValue *>(v)) {
        return c->val == val;
    }
    return false;
}

bool ConstantValue::requiresMemory() const {
    return false;
}

int ConstantValue::getValue() {
    return val;
}

VariableValue::VariableValue(string n) {
    name = n;
    version = -1;
}

VariableValue::VariableValue(string n, int v) {
    name = n;
    version = v;
}

VariableValue::~VariableValue() {}

string VariableValue::toString() const {
    if (version < 0) return name;
    stringstream ss;
    ss << name << "_" << setw(2) << setfill('0') << version;
    return ss.str();
}

bool VariableValue::equals(const Value *v) const {
    if (auto *var = dynamic_cast<const VariableValue *>(v)) {
        return var->name == name && var->version == version;
    }
    return false;
}

bool VariableValue::requiresMemory() const {
    return true;
}

string VariableValue::getVariableName() {
    return name;
}

int VariableValue::getVariableVersion() {
    return version;
}

ComputedValue::ComputedValue(int num) {
    instrNumber = num;
}

ComputedValue::~ComputedValue() {}

string ComputedValue::toString() const {
    stringstream ss;
    ss << "(" << setw(3) << instrNumber << ")";
    return ss.str();
}

bool ComputedValue::equals(const Value *v) const {
    if (auto *c = dynamic_cast<const ComputedValue *>(v)) {
        return c->instrNumber == instrNumber;
    }
    return false;
}

bool ComputedValue::requiresMemory() const {
    return true;
}

InheritedValue::InheritedValue(Value *v, int num) {
    value = v;
    blockNumber = num;
}

InheritedValue::~InheritedValue() {}

string InheritedValue::toString() const {
    stringstream ss;
    ss << setw(2) << blockNumber << ":" << setw(5) << value->toString();
    return ss.str();
}

bool InheritedValue::equals(const Value *v) const {
    if (auto *i = dynamic_cast<const InheritedValue *>(v)) {
        return value->equals(i->value) &&
            blockNumber == i->blockNumber;
    }
    return false;
}

bool InheritedValue::requiresMemory() const {
    return value->requiresMemory();
}

Value * InheritedValue::getValue() {
    return value;
}

int InheritedValue::getOriginBlockNumber() {
    return blockNumber;
}

InstructionAddressValue::InstructionAddressValue(int num) {
    blockNumber = num;
}

InstructionAddressValue::~InstructionAddressValue() {}

string InstructionAddressValue::toString() const {
    return "{" + to_string(blockNumber) + "}";
}

bool InstructionAddressValue::equals(const Value *v) const {
    if (auto *a = dynamic_cast<const InstructionAddressValue *>(v)) {
        return a->blockNumber == blockNumber;
    }
    return false;
}

bool InstructionAddressValue::requiresMemory() const {
    return false;
}

int InstructionAddressValue::getBlockNumber() {
    return blockNumber;
}

RegisterValue::RegisterValue(int num) {
    registerNumber = num;
}

RegisterValue::~RegisterValue() {}

string RegisterValue::toString() const {
    return "R" + to_string(registerNumber);
}

bool RegisterValue::equals(const Value *v) const {
    if (auto *r = dynamic_cast<const RegisterValue *>(v)) {
        return r->registerNumber == registerNumber;
    }
    return false;
}

bool RegisterValue::requiresMemory() const {
    return true;
}

int RegisterValue::getRegisterNumber() {
    return registerNumber;
}
