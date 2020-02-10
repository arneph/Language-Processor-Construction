#include "SSAGenerator.h"

SSAGenerator::SSAGenerator(Program *pgm) {
    program = pgm;
    
    generateSSA();
}

SSAGenerator::~SSAGenerator() {}

void SSAGenerator::generateSSA() {
    // Find variable definitions:
    unordered_map<string, set<BasicBlock *>> varDefs;
    
    for (BasicBlock *block : program->getBlocks()) {
        for (Instruction *instr : block->getInstructions()) {
            if (!instr->hasResult())
                continue;
            
            Value *resValue = instr->getResult();
            if (VariableValue *varValue = dynamic_cast<VariableValue *>(resValue)) {
                string name = varValue->getVariableName();
                
                varDefs[name].insert(block);
            }
        }
    }
    
    // Insert phi functions:
    int instrNum = 101;
    for (auto varDefGroup : varDefs) {
        string name = varDefGroup.first;
        set<BasicBlock *> defBlocks = varDefGroup.second;
        set<BasicBlock *> seen = set<BasicBlock *>(defBlocks);
        
        while (defBlocks.size() > 0) {
            BasicBlock *defBlock = *defBlocks.begin();
            defBlocks.erase(defBlock);
            
            for (BasicBlock *dfe : defBlock->getDominanceFrontier()) {
                if (dfe->hasPhiInstructionForVariable(name)) continue;
                
                VariableValue *resValue = new VariableValue(name);
                Instruction *phiInstr = new Instruction(instrNum++, Operator::Phi, {resValue});
                
                dfe->addPhiInstruction(phiInstr);
                
                if (seen.count(dfe) == 0) {
                    seen.insert(dfe);
                    defBlocks.insert(dfe);
                }
            }
        }
    }
    
    // Set variable versions:
    unordered_map<BasicBlock *, unordered_map<string, int>> blockVarVersions;
    
    vector<BasicBlock *> stack;
    stack.push_back(program->getStartBlock());
    
    while (stack.size() > 0) {
        BasicBlock *current = stack.back();
        BasicBlock *dominator = current->getDominator();
        stack.pop_back();
        
        for (BasicBlock *dominee : current->getDominees())
            stack.push_back(dominee);
        
        unordered_map<string, int> varVersions;
        if (dominator != nullptr) {
            varVersions = blockVarVersions[dominator];
        }else{
            for (auto it : varDefs)
                varVersions[it.first] = -1;
        }
        
        for (Instruction *instr : current->getInstructions()) {
            for (int i = 0; i < instr->getArguments().size(); i++) {
                Value *argValue = instr->getArguments()[i];
                if (auto *varValue = dynamic_cast<VariableValue *>(argValue)) {
                    string name = varValue->getVariableName();
                    int version = varVersions[name];
                    VariableValue *numberedVarValue =
                        new VariableValue(name, version);
                    
                    instr->getArguments()[i] = numberedVarValue;
                }
            }
            
            if (!instr->hasResult()) continue;
            if (auto *resValue =
                dynamic_cast<VariableValue *>(instr->getResult())) {
                string name = resValue->getVariableName();
                int version = instr->getNumber();
                
                VariableValue *numberedResValue =
                    new VariableValue(name, version);
                
                instr->getArguments()[0] = numberedResValue;
                instr->setResult(numberedResValue);
                
                varVersions[name] = version;
            }
        }
        
        blockVarVersions[current] = varVersions;
        
        for (BasicBlock *child : current->getChildren())
            for (auto it : varVersions) {
                string name = it.first;
                int version = it.second;
                Instruction *phiInstr = child->getPhiInstructionForVariable(name);
                if (phiInstr == nullptr)
                    continue;
                
                VariableValue *varValue = new VariableValue(name, version);
                InheritedValue *argValue = new InheritedValue(varValue, current->getNumber());
                
                phiInstr->getArguments().push_back(argValue);
            }
    }
}
