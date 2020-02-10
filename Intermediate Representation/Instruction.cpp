#include "Instruction.h"

Instruction::Instruction(int num,
                         Operator o,
                         initializer_list<Value *> as) {
    number = num;
    op = o;
    arguments = vector<Value *>(as);
    
    bool has_result =
        (Operator::Add <= op && op <= Operator::Compare) ||
        (Operator::Move <= op && op <= Operator::Phi);
    
    result = (has_result) ? arguments[0] : nullptr;
}

Instruction::~Instruction() {}

string Instruction::toString() {
    stringstream bob;
    bob << setw(3) << number << " ";
    bob << setw(3) << OperatorStrings[op];
    for (Value *arg : arguments) {
        bob << " " << setw(9) << arg->toString();
    }
    
    return bob.str();
}

bool Instruction::computationEquals(Instruction *instr) {
    if (instr->op != op) return false;
    if (instr->arguments.size() != arguments.size()) return false;
    for (int i = 0; i < arguments.size(); i++)
        if (!instr->arguments[i]->equals(arguments[i]))
            return false;
    return true;
}

int Instruction::getNumber() {
    return number;
}

Operator Instruction::getOperator() {
    return op;
}

vector<Value *>& Instruction::getArguments() {
    return arguments;
}

bool Instruction::hasResult() {
    return result != nullptr;
}

Value * Instruction::getResult() {
    return result;
}

void Instruction::setResult(Value *res) {
    result = res;
}
