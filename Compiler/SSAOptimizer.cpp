#include "SSAOptimizer.h"

SSAOptimizer::SSAOptimizer(Program *pgm) {
    program = pgm;
    liveRangeTable = new LiveRangeTable();
    
    moveInstructionEliminationTrace
        << "Move Instruction Elimination Trace\n";
    constantPropagationTrace
        << "Constant Propagation Trace\n";
    copyPropagationTrace
        << "Copy Propagation Trace\n";
    unusedInstructionEliminationTrace
        << "Unused Instruction Elimination Trace\n";
    
    eliminateMoveInstructions();
    propagateConstants();
    propagateCopies();
    findLiveRanges();
    eliminateUnusedInstructions();
}

SSAOptimizer::~SSAOptimizer() {}

Program * SSAOptimizer::getProgram() {
    return program;
}

LiveRangeTable * SSAOptimizer::getLiveRangeTable() {
    return liveRangeTable;
}

string SSAOptimizer::getMoveInstructionElinimationTrace() {
    return moveInstructionEliminationTrace.str();
}

string SSAOptimizer::getConstantPropagationTrace() {
    return constantPropagationTrace.str();
}

string SSAOptimizer::getCopyPropagationTrace() {
    return copyPropagationTrace.str();
}

string SSAOptimizer::getUnusedInstructionEliminationTrace() {
    return unusedInstructionEliminationTrace.str();
}

void SSAOptimizer::eliminateMoveInstructions() {
    stringstream &trace = moveInstructionEliminationTrace;
    
    unordered_map<Value *, Value *,
        decltype(ValueHash), decltype(ValueEqual)>
        replacements(0, ValueHash, ValueEqual);
    
    vector<BasicBlock *> queue;
    queue.push_back(program->getStartBlock());
    
    while (queue.size() > 0) {
        BasicBlock *current = queue.back();
        queue.pop_back();
        
        for (BasicBlock *dominee : current->getDominees()) {
            queue.push_back(dominee);
        }
        
        bool madeChanges = false;
        for (auto it = current->getInstructions().begin();
             it != current->getInstructions().end();) {
            Instruction *instr = *it;
            
            for (int j = 0; j < instr->getArguments().size(); j++) {
                Value *oldValue = instr->getArguments()[j];
                Value *newValue;
                if (auto *ihr = dynamic_cast<InheritedValue *>(oldValue)) {
                    oldValue = ihr->getValue();
                    newValue = (replacements.count(oldValue)) ?
                    new InheritedValue(replacements[oldValue],
                                       ihr->getOriginBlockNumber())
                    : ihr;
                }else{
                    newValue = (replacements.count(oldValue)) ? replacements[oldValue]
                    : oldValue;
                }
                
                instr->getArguments()[j] = newValue;
            }
            
            if (instr->getOperator() != Operator::Move) {
                ++it;
                continue;
            }
            
            Value *oldValue = instr->getArguments()[0];
            Value *newValue = instr->getArguments()[1];
            
            trace << setw(3) << setfill('0') << instr->getNumber();
            trace << ": ";
            trace << setw(5) << setfill(' ') << oldValue->toString();
            trace << " -> ";
            trace << setw(5) << setfill(' ') << newValue->toString();
            trace << endl;
            
            it = current->getInstructions().erase(it);
            replacements[oldValue] = newValue;
            madeChanges = true;
        }
        
        if (!madeChanges) continue;
        for (BasicBlock *child : current->getChildren()) {
            if (child->getDominator() == current) continue;
            
            queue.push_back(child);
        }
    }
}

Value * compute(Instruction *instr);

void SSAOptimizer::propagateConstants() {
    stringstream &trace = constantPropagationTrace;
    
    unordered_map<Value *, Value *,
        decltype(ValueHash), decltype(ValueEqual)>
        replacements(0, ValueHash, ValueEqual);
    
    vector<BasicBlock *> queue;
    queue.push_back(program->getStartBlock());
    
    while (queue.size() > 0) {
        BasicBlock *current = queue.back();
        queue.pop_back();
        
        for (BasicBlock *dominee : current->getDominees()) {
            queue.push_back(dominee);
        }
        
        bool madeChanges = false;
        for (auto it = current->getInstructions().begin();
             it != current->getInstructions().end();) {
            Instruction *instr = *it;
            
            for (int j = 0; j < instr->getArguments().size(); j++) {
                Value *oldValue = instr->getArguments()[j];
                Value *newValue;
                if (auto *ihr = dynamic_cast<InheritedValue *>(oldValue)) {
                    oldValue = ihr->getValue();
                    newValue = (replacements.count(oldValue)) ?
                        new InheritedValue(replacements[oldValue],
                                           ihr->getOriginBlockNumber())
                        : ihr;
                }else{
                    newValue = (replacements.count(oldValue)) ? replacements[oldValue]
                        : oldValue;
                }
                
                instr->getArguments()[j] = newValue;
            }
            
            if (!instr->hasResult()) {
                ++it;
                continue;
            }
            
            Value *oldResult = instr->getResult();
            Value *newResult = compute(instr);
            
            if (oldResult == newResult) {
                ++it;
                continue;
            }
            
            trace << setw(3) << setfill('0') << instr->getNumber();
            trace << ": ";
            trace << setw(5) << setfill(' ') << oldResult->toString();
            trace << " -> ";
            trace << setw(5) << setfill(' ') << newResult->toString();
            trace << endl;
            
            it = current->getInstructions().erase(it);
            replacements[oldResult] = newResult;
            madeChanges = true;
        }
        
        if (!madeChanges) continue;
        for (BasicBlock *child : current->getChildren()) {
            if (child->getDominator() == current) continue;
            
            queue.push_back(child);
        }
    }
}

Value * compute(Instruction *instr) {
    unsigned long argN = instr->getArguments().size();
    Value *argA = (argN > 1) ? instr->getArguments()[1] : nullptr;
    Value *argB = (argN > 2) ? instr->getArguments()[2] : nullptr;
    bool argAconst = false;
    bool argBconst = false;
    int argAval = 0, argBval = 0;
    if (argA == nullptr);
    else if (auto *c = dynamic_cast<ConstantValue *>(argA)) {
        argAconst = true;
        argAval = c->getValue();
    }
    if (argB == nullptr);
    else if (auto *c = dynamic_cast<ConstantValue *>(argB)) {
        argBconst = true;
        argBval = c->getValue();
    }
    
    switch (instr->getOperator()) {
        case Operator::Add:
            if (argAconst && argBconst)
                return new ConstantValue(argAval + argBval);
            if (argAconst && argAval == 0)
                return argB;
            if (argBconst && argBval == 0)
                return argA;
            break;
            
        case Operator::Subtract:
        case Operator::Compare:
            if (argAconst && argBconst)
                return new ConstantValue(argAval - argBval);
            if (argA->equals(argB))
                return new ConstantValue(0);
            if (argBconst && argBval == 0)
                return argA;
            break;
            
        case Operator::Muliply:
            if (argAconst && argBconst)
                return new ConstantValue(argAconst * argBconst);
            if (argAconst && argAval == 0)
                return new ConstantValue(0);
            if (argAconst && argAval == 1)
                return argB;
            if (argBconst && argBval == 0)
                return new ConstantValue(0);
            if (argBconst && argBval == 1)
                return argA;
            break;
            
        case Operator::Divide:
            if (argAconst && argBconst)
                return new ConstantValue(argAval / argBval);
            if (argA->equals(argB))
                return new ConstantValue(1);
            if (argBconst && argBval == 1)
                return argA;
            break;
            
        case Operator::LeftShift:
            if (argAconst && argBconst)
                return new ConstantValue(argAval << argBval);
            if (argAconst && argAval == 0)
                return new ConstantValue(0);
            if (argBconst && argBval == 0)
                return argA;
            break;
            
        case Operator::RightShift:
            if (argAconst && argBconst)
                return new ConstantValue(argAval >> argBval);
            if (argAconst && argAval == 0)
                return new ConstantValue(0);
            if (argBconst && argBval == 0)
                return argA;
            break;
            
        case Operator::Increment:
            if (argAconst)
                return new ConstantValue(argAconst + 1);
            break;
            
        case Operator::Phi: {
            bool ok = true;
            int constVal = argAval;
            for (int i = 1; i < argN; i++) {
                InheritedValue *ihv =
                    (InheritedValue *)instr->getArguments()[i];
                if (auto *c = dynamic_cast<ConstantValue *>(ihv->getValue())) {
                    int val = c->getValue();
                    if (val != constVal) {
                        ok = false;
                        break;
                    }
                }else{
                    ok = false;
                    break;
                }
            }
            if (ok)
                return argA;
            break;
        }
        default:
            break;
    }
    
    return instr->getResult();
}

bool isArithmeticOperator(Operator op) {
    return Operator::Add <= op && op <= Operator::Increment;
}

bool canReuseInstruction(Instruction *oldInstr, Instruction *newInstr);

void SSAOptimizer::propagateCopies() {
    stringstream &trace = copyPropagationTrace;
    
    unordered_map<Value *, Value *,
        decltype(ValueHash), decltype(ValueEqual)>
        replacements(0, ValueHash, ValueEqual);
    
    unordered_map<BasicBlock *,
        unordered_map<Operator, vector<Instruction *>>>
        reusableInstructions;
    
    vector<BasicBlock *> queue;
    queue.push_back(program->getStartBlock());
    
    while (queue.size() > 0) {
        BasicBlock *current = queue.back();
        BasicBlock *dominator = current->getDominator();
        queue.pop_back();
        
        for (BasicBlock *dominee : current->getDominees()) {
            queue.push_back(dominee);
        }
        
        unordered_map<Operator, vector<Instruction *>> reuseInstrs;
        if (reusableInstructions.count(dominator) > 0) {
            reuseInstrs = reusableInstructions[dominator];
        }
        
        bool madeChanges = false;
        for (auto it = current->getInstructions().begin();
             it != current->getInstructions().end();) {
            Instruction *instr = *it;
            Operator op = instr->getOperator();
            
            for (int j = 0; j < instr->getArguments().size(); j++) {
                Value *oldValue = instr->getArguments()[j];
                Value *newValue;
                if (auto *ihr = dynamic_cast<InheritedValue *>(oldValue)) {
                    oldValue = ihr->getValue();
                    newValue = (replacements.count(oldValue)) ?
                    new InheritedValue(replacements[oldValue],
                                       ihr->getOriginBlockNumber())
                    : ihr;
                }else{
                    newValue = (replacements.count(oldValue)) ? replacements[oldValue]
                    : oldValue;
                }
                
                instr->getArguments()[j] = newValue;
            }
            
            if (!isArithmeticOperator(op)) {
                ++it;
                continue;
            }
            
            Instruction *reuseInstr = nullptr;
            for (auto it = reuseInstrs[op].begin();
                 it != reuseInstrs[op].end(); ++it) {
                if (canReuseInstruction(instr, *it)) {
                    reuseInstr = *it;
                    break;
                }
            }
            
            reuseInstrs[op].push_back(instr);
            
            if (reuseInstr == nullptr) {
                ++it;
                continue;
            }
            
            Value *oldValue = instr->getResult();
            Value *newValue = reuseInstr->getResult();
            
            trace << setw(3) << setfill('0') << instr->getNumber();
            trace << ": ";
            trace << setw(5) << setfill(' ') << oldValue->toString();
            trace << " -> ";
            trace << setw(5) << setfill(' ') << newValue->toString();
            trace << endl;
            
            it = current->getInstructions().erase(it);
            replacements[oldValue] = newValue;
            madeChanges = true;
        }
        
        reusableInstructions[current] = reuseInstrs;
        
        if (!madeChanges) continue;
        for (BasicBlock *child : current->getChildren()) {
            if (child->getDominator() == current) continue;
            
            queue.push_back(child);
        }
    }
}

bool canReuseInstruction(Instruction *oldInstr, Instruction *newInstr) {
    if (oldInstr->getOperator() != newInstr->getOperator()) {
        return false;
    }
    
    unsigned long oldArgN = oldInstr->getArguments().size();
    unsigned long newArgN = newInstr->getArguments().size();
    
    if (oldArgN != newArgN) {
        return false;
    }
    
    Value *oldArgA = (oldArgN > 1) ? oldInstr->getArguments()[1] : nullptr;
    Value *oldArgB = (oldArgN > 2) ? oldInstr->getArguments()[2] : nullptr;
    Value *newArgA = (newArgN > 1) ? newInstr->getArguments()[1] : nullptr;
    Value *newArgB = (newArgN > 2) ? newInstr->getArguments()[2] : nullptr;
    
    bool eqOldANewA = (oldArgN > 1) ? oldArgA->equals(newArgA) : false;
    bool eqOldANewB = (oldArgN > 2) ? oldArgA->equals(newArgB) : false;
    bool eqOldBNewA = (oldArgN > 2) ? oldArgB->equals(newArgA) : false;
    bool eqOldBNewB = (oldArgN > 2) ? oldArgB->equals(newArgB) : false;
    bool eqArgMatch = eqOldANewA && eqOldBNewB;
    bool eqArgSets = (eqOldANewA && eqOldBNewB) || (eqOldANewB && eqOldBNewA);
    
    switch (oldInstr->getOperator()) {
        case Operator::Add:
            return eqArgSets;
        
        case Operator::Subtract:
            return eqArgMatch;
            
        case Operator::Muliply:
            return eqArgSets;
            
        case Operator::Divide:
            return eqArgMatch;
            
        case Operator::Negative:
            return eqOldANewA;
            
        case Operator::LeftShift:
        case Operator::RightShift:
            return eqArgMatch;
            
        case Operator::Increment:
            return eqOldANewA;
        /*
        case Operator::Compare:
            return eqArgMatch;*/
            
        default:
            return false;
    }
}

void SSAOptimizer::findLiveRanges() {
    vector<BasicBlock *> queue;
    queue.push_back(program->getStartBlock());
    
    while (queue.size() > 0) {
        BasicBlock *current = queue.back();
        queue.pop_back();
        
        for (BasicBlock *dominee : current->getDominees()) {
            queue.push_back(dominee);
        }
        
        for (auto it = current->getInstructions().rbegin();
             it != current->getInstructions().rend(); ++it) {
            Instruction *instr = *it;
            int num = instr->getNumber();
            Value *result = instr->getResult();
            
            if (result != nullptr) {
                liveRangeTable->setDefinition(result, num);
            }
            
            for (int j = 0; j < instr->getArguments().size(); j++) {
                Value *argValue = instr->getArguments()[j];
                
                if (InheritedValue *ih =
                    dynamic_cast<InheritedValue *>(argValue)) {
                    argValue = ih->getValue();
                }
                
                if (argValue->requiresMemory() &&
                    argValue != result) {
                    liveRangeTable->addUse(argValue, num);
                }
            }
        }
    }
}

void SSAOptimizer::eliminateUnusedInstructions() {
    stringstream &trace = unusedInstructionEliminationTrace;
    
    bool madeChanges = true;
    while (madeChanges) {
        madeChanges = false;
        
        for (BasicBlock *block : program->getBlocks()) {
            for (auto it = block->getInstructions().begin();
                 it != block->getInstructions().end();) {
                Instruction *instr = *it;
                int num = instr->getNumber();
                Value *result = instr->getResult();
                
                if (result == nullptr ||
                    liveRangeTable->getUseCount(result) > 0) {
                    ++it;
                    continue;
                }
                
                trace << setw(3) << setfill('0') << num;
                trace << ": ";
                trace << setw(5) << setfill(' ') << result->toString();
                trace << endl;
                
                for (Value *argValue : instr->getArguments()) {
                    if (argValue == result) continue;
                    if (InheritedValue *ih =
                        dynamic_cast<InheritedValue *>(argValue)) {
                        argValue = ih->getValue();
                    }
                    if (argValue->requiresMemory()) {
                        liveRangeTable->removeUse(argValue, num);
                    }
                }
                
                it = block->getInstructions().erase(it);
                liveRangeTable->removeValue(result);
                madeChanges = true;
            }
        }
    }
}
