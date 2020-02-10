#include "BasicBlock.h"

BasicBlock::BasicBlock(int num) {
    number = num;
    address = new InstructionAddressValue(num);
}

BasicBlock::~BasicBlock() {}

string BasicBlock::getTitle() {
    return "{" + to_string(number) + "}";
}

string BasicBlock::toString() {
    stringstream ss;
    ss << "{" << number << "}";
    for (Instruction *instr : instructions) {
        ss << endl << instr->toString();
    }
    return ss.str();
}

int BasicBlock::getNumber() {
    return number;
}

const vector<BasicBlock *>& BasicBlock::getParents() {
    return parents;
}

const vector<BasicBlock *>& BasicBlock::getChildren() {
    return children;
}

BasicBlock * BasicBlock::getDominator() {
    return dominator;
}

const set<BasicBlock *>& BasicBlock::getDominees() {
    return dominees;
}

const set<BasicBlock *>& BasicBlock::getDominanceFrontier() {
    return dominanceFrontier;
}

vector<Instruction *>& BasicBlock::getInstructions() {
    return instructions;
}

bool BasicBlock::hasPhiInstructionForVariable(string variable) {
    return getPhiInstructionForVariable(variable) != nullptr;
}

Instruction * BasicBlock::getPhiInstructionForVariable(string variable) {
    for (auto it = instructions.begin(); it != instructions.end(); ++it) {
        if ((*it)->getOperator() != Operator::Phi) break;
        
        Value *resValue = (*it)->getResult();
        if (VariableValue *varValue = dynamic_cast<VariableValue *>(resValue))
            if (varValue->getVariableName() == variable)
                return *it;
    }
    
    return nullptr;
}

void BasicBlock::addInstruction(Instruction *instr) {
    instructions.push_back(instr);
}

void BasicBlock::addPhiInstruction(Instruction *instr) {
    for (auto it = instructions.begin(); it != instructions.end(); ++it) {
        if ((*it)->getOperator() != Operator::Phi) {
            instructions.insert(it, instr);
            break;
        }
    }
}

InstructionAddressValue * BasicBlock::getAddress() {
    return address;
}
