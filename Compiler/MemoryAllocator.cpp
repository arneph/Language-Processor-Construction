#include "MemoryAllocator.h"

MemoryAllocator::MemoryAllocator(Program *pgm, LiveRangeTable *t) {
    program = pgm;
    liveRangeTable = t;
    
    buildInterferenceGraph();
    colorInterferenceGraph();
    applyInterferenceGraphColors();
    resolvePhiInstructions();
}

MemoryAllocator::~MemoryAllocator() {}

VCGGraph * MemoryAllocator::getInterferenceGraph() {
    VCGGraph *g = new VCGGraph;
    
    unordered_map<Value *, VCGNode *, decltype(ValueHash), decltype(ValueEqual)>nodes(0, ValueHash, ValueEqual);
    unordered_map<Value *, int, decltype(ValueHash), decltype(ValueEqual)>indices(0, ValueHash, ValueEqual);
    
    int i = 0;
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        Value *value = it->first;
        string text = value->toString();
        int color = colors[value];
        VCGNode *node = new VCGNode(text, color);
        g->addNode(node);
        
        nodes[value] = node;
        indices[value] = i++;
    }
    
    for (auto itA = graph.begin();
         itA != graph.end(); ++itA) {
        Value *valueA = itA->first;
        VCGNode *nodeA = nodes[valueA];
        int indexA = indices[valueA];
        
        for (auto itB = itA->second->begin();
             itB != itA->second->end(); ++itB) {
            Value *valueB = *itB;
            VCGNode *nodeB = nodes[valueB];
            int indexB = indices[valueB];
            
            if (indexA >= indexB)
                continue;
            
            VCGEdge *edge = new VCGEdge(nodeA, nodeB, false);
            
            g->addEdge(edge);
        }
    }
    
    return g;
}

string MemoryAllocator::getValueAllocationTable() {
    return valueAllocationTable.str();
}

void MemoryAllocator::addNode(Value *value) {
    if (graph.count(value)) {
        return;
    }
    
    graph[value] = new unordered_set
        <Value *, decltype(ValueHash), decltype(ValueEqual)>
        (0, ValueHash, ValueEqual);
}

void MemoryAllocator::addEdge(Value *valueA, Value *valueB) {
    if (valueA->equals(valueB)) {
        return;
    }
    if (!graph.count(valueA)) {
        addNode(valueA);
    }
    if (!graph.count(valueB)) {
        addNode(valueB);
    }
    
    graph[valueA]->insert(valueB);
    graph[valueB]->insert(valueA);
}

void MemoryAllocator::buildInterferenceGraph() {
    unordered_map<BasicBlock *,
        unordered_set<Value *,
            decltype(ValueHash),
            decltype(ValueEqual)> *>
        endLiveSets;
    
    for (BasicBlock *block :program->getBlocks()) {
        endLiveSets[block] = new unordered_set<Value *,
            decltype(ValueHash),
            decltype(ValueEqual)>
            (0, ValueHash, ValueEqual);
    }
    
    for (BasicBlock *block : program->getBlocks()) {
        for (Instruction *instr : block->getInstructions()) {
            if (instr->getOperator() != Operator::Phi) {
                break;
            }
            
            for (Value *arg : instr->getArguments()) {
                if (InheritedValue *ih =
                    dynamic_cast<InheritedValue *>(arg)) {
                    int blockNum = ih->getOriginBlockNumber();
                    Value *value = ih->getValue();
                    
                    if (!value->requiresMemory()) {
                        continue;
                    }
                    
                    BasicBlock *parentBlock = program->getBlocks()[blockNum];
                    
                    endLiveSets[parentBlock]->insert(value);
                }
            }
        }
    }
    
    vector<BasicBlock *> domOrder;
    vector<BasicBlock *> queue;
    unordered_set<BasicBlock *>queueSet;
    queue.push_back(program->getStartBlock());
    while (queue.size() > 0) {
        BasicBlock *current = queue.back();
        queue.pop_back();
        
        for (BasicBlock *dominee : current->getDominees()) {
            queue.push_back(dominee);
        }
        
        domOrder.push_back(current);
    }
    
    for (auto itA = domOrder.rbegin();
         itA != domOrder.rend(); ++itA) {
        queue.push_back(*itA);
        queueSet.insert(*itA);
    }
    
    while (queue.size() > 0) {
        BasicBlock *block = queue.front();
        queue.erase(queue.begin());
        queueSet.erase(block);
        
        unordered_set<Value *,
                decltype(ValueHash),
                decltype(ValueEqual)>
            liveSet(0, ValueHash, ValueEqual);
        for (Value *value : *endLiveSets[block]) {
            liveSet.insert(value);
        }
        
        for (Value *valueA : liveSet) {
            for (Value *valueB :liveSet) {
                addNode(valueA);
                addNode(valueB);
                addEdge(valueA, valueB);
            }
        }
        
        for (auto itB = block->getInstructions().rbegin();
             itB != block->getInstructions().rend(); ++itB) {
            Instruction *instr = *itB;
            int num = instr->getNumber();
            
            Value *definedValue = liveRangeTable->getDefinedValue(num);
            
            if (definedValue != nullptr) {
                liveSet.erase(liveSet.find(definedValue));
            }
            
            if (instr->getOperator() != Operator::Phi) {
                for (Value *usedValue : liveRangeTable->getUsedValues(num)) {
                    liveSet.insert(usedValue);
                }
                
                for (Value *valueA : liveSet) {
                    for (Value *valueB : liveSet) {
                        addNode(valueA);
                        addNode(valueB);
                        addEdge(valueA, valueB);
                    }
                }
                
            }else{
                for (Value *valueA : liveRangeTable->getUsedValues(num)) {
                    for (Value *valueB : liveSet) {
                        addNode(valueA);
                        addNode(valueB);
                        addEdge(valueA, valueB);
                    }
                }
            }
        }
        
        for (BasicBlock *parent : block->getParents()) {
            unsigned long oldSize = endLiveSets[parent]->size();
            for (Value *value : liveSet) {
                endLiveSets[parent]->insert(value);
            }
            unsigned long newSize = endLiveSets[parent]->size();
            if (oldSize == newSize) {
                continue;
            }
            
            if (queueSet.count(parent)) {
                continue;
            }
            queue.push_back(parent);
            queueSet.insert(parent);
        }
    }
}

void MemoryAllocator::colorInterferenceGraph() {
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        colors[it->first] = -1;
    }
    
    for (int i = 0; i < graph.size(); i++) {
        Value *nextValue = nullptr;
        unsigned long nextConstraints = 0;
        
        int mostNeighborsCount = -1;
        int mostConstraintsCount = -1;
        for (auto it = graph.begin(); it != graph.end(); ++it) {
            if (colors[it->first] != -1) continue;
            
            unsigned long constraints = 0;
            for (Value *neighbor : *(it->second)) {
                if (colors[neighbor] == -1) continue;
                constraints |= (1 << colors[neighbor]);
            }
            
            int neighborsCount = (int)it->second->size();
            int constraintsCount = 0;
            for (int j = 0; j < 64; j++) {
                constraintsCount += (constraints >> j) & 0x1;
            }
            
            if (mostNeighborsCount < neighborsCount ||
                (mostNeighborsCount == neighborsCount &&
                 mostConstraintsCount < constraintsCount)) {
                mostNeighborsCount = neighborsCount;
                mostConstraintsCount = constraintsCount;
                nextConstraints = constraints;
                nextValue = it->first;
            }
        }
        
        for (int j = 0; j < 64; j++) {
            if (nextConstraints & (1 << j)) continue;
            
            colors[nextValue] = j;
            break;
        }
    }
}

void MemoryAllocator::applyInterferenceGraphColors() {
    for (BasicBlock *block : program->getBlocks()) {
        for (auto it = block->getInstructions().begin();
             it != block->getInstructions().end(); ++it) {
            Instruction *instr = *it;
            
            for (int j = 0; j < instr->getArguments().size(); j++) {
                Value *oldValue = instr->getArguments()[j];
                
                if (auto *ihr = dynamic_cast<InheritedValue *>(oldValue)) {
                    oldValue = ihr->getValue();
                    if (colors.count(oldValue) == 0) {
                        continue;
                    }
                    
                    Value *newValue = new RegisterValue(colors[oldValue]);
                    Value *newInheritedValue =
                        new InheritedValue(newValue, ihr->getOriginBlockNumber());
                    
                    instr->getArguments()[j] = newInheritedValue;
                    
                }else{
                    if (colors.count(oldValue) == 0) {
                        continue;
                    }
                    
                    Value *newValue = new RegisterValue(colors[oldValue]);
                    
                    instr->getArguments()[j] = newValue;
                }
            }
        }
    }
}

void MemoryAllocator::resolvePhiInstructions() {
    unsigned long n = program->getBlocks().size();
    int instrCount = 201;
    for (int i = 0; i < n; i++) {
        BasicBlock *block = program->getBlocks()[i];
        
        if (block->getInstructions().size() < 1) continue;
        if (block->getInstructions()[0]->getOperator() != Operator::Phi) continue;
        
        vector<BasicBlock *> oldParents(block->getParents());
        for (BasicBlock *parent : oldParents) {
            int inBlockNum = parent->getNumber();
            
            if (parent->getChildren().size() > 1) {
                BasicBlock *jumpBlock = program->addBlock();
                
                Instruction *braInstr = parent->getInstructions().back();
                InstructionAddressValue *addrA =
                    (InstructionAddressValue *) braInstr->getArguments()[1];
                InstructionAddressValue *addrB =
                    (InstructionAddressValue *) braInstr->getArguments()[2];
                if (block->getNumber() == addrA->getBlockNumber()) {
                    braInstr->getArguments()[1] = jumpBlock->getAddress();
                }else if (block->getNumber() == addrB->getBlockNumber()) {
                    braInstr->getArguments()[2] = jumpBlock->getAddress();
                }
                
                braInstr = new Instruction(instrCount++,
                                           Operator::Branch,
                                           {block->getAddress()});
                jumpBlock->addInstruction(braInstr);
                
                program->removeCFGEdge(parent, block);
                program->addCFGEdge(parent, jumpBlock);
                program->addCFGEdge(jumpBlock, block);
                
                parent = jumpBlock;
            }
            
            auto itA = parent->getInstructions().begin();
            for (int j = (int)parent->getInstructions().size() - 1; j >= 0; j--) {
                Instruction *instr = parent->getInstructions()[j];
                Operator op = instr->getOperator();
                if (Operator::Branch <= op && op <= Operator::BranchLessThan) {
                    continue;
                }
                
                itA = parent->getInstructions().begin() + (j + 1);
            }
            
            for (auto itB = block->getInstructions().begin();
                 itB != block->getInstructions().end(); ++itB) {
                Instruction *phiInstr = *itB;
                
                if (phiInstr->getOperator() != Operator::Phi) {
                    break;
                }
                
                Value *dest = phiInstr->getArguments()[0];
                Value *orig = nullptr;
                
                for (int j = 0; j < oldParents.size(); j++) {
                    InheritedValue *ih =
                        (InheritedValue *) phiInstr->getArguments()[j + 1];
                    if (ih->getOriginBlockNumber() != inBlockNum) continue;
                    
                    orig = ih->getValue();
                }
                
                if (dest->equals(orig)) {
                    // Yaaay! Dodged a move instruction!
                    continue;
                }
                
                Instruction *movInstr = new Instruction(instrCount++,
                                                        Operator::Move,
                                                        {dest, orig});
                itA = parent->getInstructions().insert(itA, movInstr)+1;
            }
        }
        
        for (auto it = block->getInstructions().begin();
             it != block->getInstructions().end();) {
            Instruction *phiInstr = *it;
            
            if (phiInstr->getOperator() != Operator::Phi) {
                break;
            }
            
            it = block->getInstructions().erase(it);
        }
    }
}
