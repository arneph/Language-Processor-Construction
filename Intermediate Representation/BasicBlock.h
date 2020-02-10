#ifndef BasicBlock_h
#define BasicBlock_h

#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "Instruction.h"

using namespace std;

class BasicBlock {
    int number;
    
    // Control Flow Graph (CFG):
    vector<BasicBlock *> parents;
    vector<BasicBlock *> children;
    
    // Dominator Tree:
    BasicBlock * dominator;
    set<BasicBlock *> dominees;
    set<BasicBlock *> dominanceFrontier;
    
    vector<Instruction *> instructions;
    
    InstructionAddressValue *address;
    
    friend class Program;
    
public:
    BasicBlock(int num);
    ~BasicBlock();
    
    string getTitle();
    string toString();
    
    int getNumber();
    
    const vector<BasicBlock *>& getParents();
    const vector<BasicBlock *>& getChildren();
    
    BasicBlock * getDominator();
    const set<BasicBlock *>& getDominees();
    const set<BasicBlock *>& getDominanceFrontier();
    
    vector<Instruction *>& getInstructions();
    bool hasPhiInstructionForVariable(string variable);
    Instruction * getPhiInstructionForVariable(string variable);
    void addInstruction(Instruction *instr);
    void addPhiInstruction(Instruction *instr);
    
    InstructionAddressValue * getAddress();
};

#endif /* BasicBlock_h */
