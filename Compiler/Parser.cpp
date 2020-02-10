#include "Parser.h"

Parser::Parser(JavaClass *c) {
    javaClass = c;
    ssaProgram = new Program();
    
    parse();
    determineDomTree();
    determineDomFrontiers();
}

Parser::~Parser() {}

Program * Parser::getProgram() {
    return ssaProgram;
}

Operator javaArithOpcodeToSSAOperator(u1 opcode);
Operator javaBranchOpcodeToSSAOperator(u1 opcode);

void Parser::parse() {
    JavaMethod *javaMain = javaClass->getMainMethod();
    JavaCodeAttribute *javaCodeAttr = javaMain->getCodeAttribute();
    vector<u1> code = javaCodeAttr->getCode();
    
    map<int, BasicBlock *> leaders;
    leaders[0] = nullptr;
    for (int i = 0; i < code.size(); i++) {
        u1 opcode = code[i];
        
        if (opcode == ifne.code ||
            opcode == ifeq.code ||
            opcode == iflt.code ||
            opcode == ifle.code ||
            opcode == ifge.code ||
            opcode == ifgt.code ||
            opcode == if_icmpne.code ||
            opcode == if_icmpeq.code ||
            opcode == if_icmplt.code ||
            opcode == if_icmple.code ||
            opcode == if_icmpge.code ||
            opcode == if_icmpgt.code ||
            opcode == goto_.code) {
            u1 branch_b1 = code[i+1];
            u1 branch_b2 = code[i+2];
            short branch =
                ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
            leaders[i + branch] = nullptr;
            if (opcode != goto_.code)
                leaders[i + 3] = nullptr;
        }
        
        i += InstrTable[opcode].argc;
    }
    
    for (map<int, BasicBlock *>::iterator it = leaders.begin();
         it != leaders.end(); ++it) {
        it->second = ssaProgram->addBlock();
    }
    
    int stackDepth = 0;
    int instrCount = 0;
    BasicBlock *currBlock = nullptr;
    for (int i = 0; i < code.size(); i++) {
        if (leaders.count(i) > 0) {
            currBlock = leaders[i];
        }
        
        u1 opcode = code[i];
        if (opcode == iconst_m1.code ||
            opcode == iconst_0.code ||
            opcode == iconst_1.code ||
            opcode == iconst_2.code ||
            opcode == iconst_3.code ||
            opcode == iconst_4.code ||
            opcode == iconst_4.code ||
            opcode == iconst_5.code ||
            opcode == bipush.code) {
            u1 data = (opcode == bipush.code) ?
                code[i + 1] : (int)(opcode) - 3;
            ConstantValue *cv =
                new ConstantValue(data);
            VariableValue *vv =
                new VariableValue("S" + to_string(stackDepth++));
            Instruction *moveInstr =
                new Instruction(instrCount++,
                                Operator::Move,
                                {vv, cv});
            currBlock->addInstruction(moveInstr);
        }else if (opcode == iload.code ||
                  opcode == iload_0.code ||
                  opcode == iload_1.code ||
                  opcode == iload_2.code ||
                  opcode == iload_3.code) {
            u1 index = (opcode == iload.code) ?
                code[i + 1] : opcode - iload_0.code;
            VariableValue *av =
                new VariableValue("S" + to_string(stackDepth++));
            VariableValue *bv =
                new VariableValue("L" + to_string(index));
            Instruction *moveInstr =
                new Instruction(instrCount++,
                                Operator::Move,
                                {av, bv});
            currBlock->addInstruction(moveInstr);
        }else if (opcode == istore.code ||
                  opcode == istore_0.code ||
                  opcode == istore_1.code ||
                  opcode == istore_2.code ||
                  opcode == istore_3.code) {
            u1 index = (opcode == istore.code) ?
                code[i + 1] : opcode - istore_0.code;
            VariableValue *av =
                new VariableValue("S" + to_string(--stackDepth));
            VariableValue *bv =
                new VariableValue("L" + to_string(index));
            Instruction *moveInstr =
                new Instruction(instrCount++,
                                Operator::Move,
                                {bv, av});
            currBlock->addInstruction(moveInstr);
        }else if (opcode == iadd.code ||
                  opcode == isub.code ||
                  opcode == imul.code ||
                  opcode == idiv.code ||
                  opcode == ishl.code ||
                  opcode == ishr.code) {
            VariableValue *av =
                new VariableValue("S" + to_string(--stackDepth));
            VariableValue *bv =
                new VariableValue("S" + to_string(--stackDepth));
            VariableValue *rv =
                new VariableValue("S" + to_string(stackDepth++));
            Instruction *addInstr =
                new Instruction(instrCount++,
                                javaArithOpcodeToSSAOperator(opcode),
                                {rv, av, bv});
            currBlock->addInstruction(addInstr);
        }else if (opcode == ineg.code) {
            VariableValue *av =
                new VariableValue("S" + to_string(--stackDepth));
            VariableValue *rv =
                new VariableValue("S" + to_string(stackDepth++));
            Instruction *negInstr =
                new Instruction(instrCount++,
                                javaArithOpcodeToSSAOperator(opcode),
                                {rv, av});
            currBlock->addInstruction(negInstr);
        }else if (opcode == iinc.code) {
            u1 index = code[i + 1];
            u1 data = code[i + 2];
            ConstantValue *cv =
                new ConstantValue(data);
            VariableValue *vv =
                new VariableValue("L" + to_string(index));
            Instruction *subInstr =
                new Instruction(instrCount++,
                                Operator::Add,
                                {vv, vv, cv});
            currBlock->addInstruction(subInstr);
        }else if (opcode == ifne.code ||
                  opcode == ifeq.code ||
                  opcode == iflt.code ||
                  opcode == ifle.code ||
                  opcode == ifge.code ||
                  opcode == ifgt.code) {
            u1 branch_b1 = code[i+1];
            u1 branch_b2 = code[i+2];
            short branch =
                ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
            
            VariableValue *dv =
                new VariableValue("S" + to_string(--stackDepth));
            InstructionAddressValue *bv =
                leaders[i + branch]->getAddress();
            InstructionAddressValue *cv =
                leaders[i + 3]->getAddress();
            Instruction *braInstr =
                new Instruction(instrCount++,
                                javaBranchOpcodeToSSAOperator(opcode),
                                {dv, bv, cv});
            currBlock->addInstruction(braInstr);
            
            ssaProgram->addCFGEdge(currBlock, leaders[i + branch]);
            ssaProgram->addCFGEdge(currBlock, leaders[i + 3]);
        }else if (opcode == if_icmpne.code ||
                  opcode == if_icmpeq.code ||
                  opcode == if_icmplt.code ||
                  opcode == if_icmple.code ||
                  opcode == if_icmpge.code ||
                  opcode == if_icmpgt.code) {
            u1 branch_b1 = code[i+1];
            u1 branch_b2 = code[i+2];
            short branch =
                ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
            
            VariableValue *xv =
                new VariableValue("S" + to_string(--stackDepth));
            VariableValue *yv =
                new VariableValue("S" + to_string(--stackDepth));
            VariableValue *dv = new VariableValue("T");
            Instruction *cmpInstr =
                new Instruction(instrCount++,
                                Operator::Compare,
                                {dv, xv, yv});
            currBlock->addInstruction(cmpInstr);
            
            InstructionAddressValue *bv =
                leaders[i + branch]->getAddress();
            InstructionAddressValue *cv =
                leaders[i + 3]->getAddress();
            Instruction *braInstr =
                new Instruction(instrCount++,
                                javaBranchOpcodeToSSAOperator(opcode),
                                {dv, bv, cv});
            currBlock->addInstruction(braInstr);
            
            ssaProgram->addCFGEdge(currBlock, leaders[i + branch]);
            ssaProgram->addCFGEdge(currBlock, leaders[i + 3]);
        }else if (opcode == goto_.code) {
            u1 branch_b1 = code[i+1];
            u1 branch_b2 = code[i+2];
            short branch =
                ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
            InstructionAddressValue *bv =
                leaders[i + branch]->getAddress();
            Instruction *braInstr =
                new Instruction(instrCount++,
                                Operator::Branch,
                                {bv});
            currBlock->addInstruction(braInstr);
            
            ssaProgram->addCFGEdge(currBlock, leaders[i + branch]);
        }else if (opcode == invokestatic.code ||
                  opcode == invokedynamic.code) {
            VariableValue *xv =
                new VariableValue("S" + to_string(--stackDepth));
            Instruction *callInstr =
                new Instruction(instrCount++,
                                Operator::ProcedureCall,
                                {xv});
            currBlock->addInstruction(callInstr);
        }else if (opcode == ireturn.code ||
                  opcode == return_.code) {
            if (opcode == ireturn.code) {
                VariableValue *xv =
                    new VariableValue("S" + to_string(--stackDepth));
                Instruction *retInstr =
                    new Instruction(instrCount++,
                                    Operator::Return,
                                    {xv});
                currBlock->addInstruction(retInstr);
            }
            if (i < code.size() - 1) {
                Instruction *endInstr =
                new Instruction(instrCount++,
                                Operator::End, {});
                currBlock->addInstruction(endInstr);
            }
        }else{
            cerr << "unknown opcode" << endl;
        }
        
        i += InstrTable[opcode].argc;
    }
    
    Instruction *endInstr =
        new Instruction(instrCount++,
                        Operator::End, {});
    currBlock->addInstruction(endInstr);
    
    for (int i = 0; i < ssaProgram->getBlocks().size() - 1; i++) {
        BasicBlock *curr = ssaProgram->getBlocks()[i];
        BasicBlock *next = ssaProgram->getBlocks()[i + 1];
        
        if (curr->getChildren().size() > 0) {
            continue;
        }
        
        ssaProgram->addCFGEdge(curr, next);
    }
}

Operator javaArithOpcodeToSSAOperator(u1 opcode) {
    if (opcode == iadd.code) {
        return Operator::Add;
    }else if (opcode == isub.code) {
        return Operator::Subtract;
    }else if (opcode == imul.code) {
        return Operator::Muliply;
    }else if (opcode == idiv.code) {
        return Operator::Divide;
    }else if (opcode == ishl.code) {
        return Operator::LeftShift;
    }else if (opcode == ishr.code) {
        return Operator::RightShift;
    }else if (opcode == ineg.code) {
        return Operator::Negative;
    }else if (opcode == iinc.code) {
        return Operator::Increment;
    }else{
        return Operator(-1);
    }
}

Operator javaBranchOpcodeToSSAOperator(u1 opcode) {
    if (opcode == ifne.code ||
        opcode == if_icmpne.code) {
        return Operator::BranchNotEqual;
    }else if (opcode == ifeq.code ||
              opcode == if_icmpeq.code) {
        return Operator::BranchEqual;
    }else if (opcode == iflt.code ||
              opcode == if_icmplt.code) {
        return Operator::BranchLessThan;
    }else if (opcode == ifle.code ||
              opcode == if_icmple.code) {
        return Operator::BranchLessThanOrEqual;
    }else if (opcode == ifge.code ||
              opcode == if_icmpge.code) {
        return Operator::BranchGreaterThanOrEqual;
    }else if (opcode == ifgt.code ||
              opcode == if_icmpgt.code) {
        return Operator::BranchGreaterThan;
    }else if (opcode == goto_.code) {
        return Operator::Branch;
    }else{
        return Operator(-1);
    }
}

void Parser::determineDomTree() {
    // Prepare Data Structures:
    unsigned long n = ssaProgram->getBlocks().size();
    treeOrder = vector<int>(n, -1);
    treeParent = vector<int>(n, -1);
    sdom = vector<int>(n, -1);
    idom = vector<int>(n, -1);
    bucket = vector<set<int>>(n, set<int>());
    ancestor = vector<int>(n, -1);
    label = vector<int>(n, -1);
    
    // Step 1: Depth-first search
    findDFSTree();
    
    // Step 2: Compute all semidominators
    // Step 3: Implicitely define immediate dominator of each basic block
    findImplicitIDoms();
    
    // Step 4: Explicitely define immediate dominator of each basic block
    findExplicitIDoms();
    
    // Update Basic Blocks:
    for (int i = 1; i < treeOrder.size(); i++) {
        int v = treeOrder[i];
        int w = idom[v];
        
        BasicBlock *dominator = ssaProgram->getBlocks()[w];
        BasicBlock *dominee = ssaProgram->getBlocks()[v];
        
        ssaProgram->addDomTreeEdge(dominator, dominee);
    }
}

void Parser::findDFSTree() {
    vector<int> stack;
    set<int> seen;
    int i = 0;
    
    stack.push_back(0);
    seen.insert(0);
    while (stack.size() > 0) {
        int v = stack.back();
        stack.pop_back();
        
        treeOrder[i] = v;
        sdom[v] = i;
        label[v] = v;
        i++;
        
        BasicBlock *block = ssaProgram->getBlocks()[v];
        for (BasicBlock *child : block->getChildren()) {
            int w = child->getNumber();
            if (seen.count(w)) continue;
            
            stack.push_back(w);
            seen.insert(w);
            
            treeParent[w] = v;
        }
    }
}

void Parser::link(int v, int w) {
    ancestor[w] = v;
}

void Parser::compress(int v) {
    if (ancestor[ancestor[v]] == -1) return;
    compress(ancestor[v]);
    if (sdom[label[ancestor[v]]] < sdom[label[v]]) {
        label[v] = label[ancestor[v]];
    }
    ancestor[v] =  ancestor[ancestor[v]];
}

int Parser::eval(int v) {
    if (ancestor[v] == -1) {
        return v;
    }
    
    compress(v);
    
    return label[v];
}

void Parser::findImplicitIDoms() {
    for (int i = (int)treeOrder.size() - 1; i > 0; i--) {
        int w = treeOrder[i];
        
        BasicBlock *block = ssaProgram->getBlocks()[w];
        
        // Step 2:
        for (BasicBlock *parent : block->getParents()) {
            int v = parent->getNumber();
            int u = eval(v);
            
            if (sdom[w] > sdom[u])
                sdom[w] = sdom[u];
        }
        
        bucket[treeOrder[sdom[w]]].insert(w);
        link(treeParent[w], w);
        
        // Step 3:
        for (int v : bucket[treeParent[w]]) {
            int u = eval(v);
            
            idom[v] = (sdom[u] < sdom[v]) ? u : treeParent[w];
        }
        bucket[treeParent[w]].clear();
    }
}

void Parser::findExplicitIDoms() {
    for (int i = 1; i < treeOrder.size(); i++) {
        int w = treeOrder[i];
        if (idom[w] != treeOrder[sdom[w]]) {
            idom[w] = idom[idom[w]];
        }
    }
    
    idom[0] = 0;
}

void Parser::determineDomFrontiers() {
    vector<BasicBlock *> domTreeDFSOrder;
    vector<BasicBlock *> stack;
    stack.push_back(ssaProgram->getStartBlock());
    
    while (stack.size() > 0) {
        BasicBlock *current = stack.back();
        stack.pop_back();
        
        domTreeDFSOrder.push_back(current);
        
        for (BasicBlock *dominee : current->getDominees()) {
            stack.push_back(dominee);
        }
    }
    
    for (int i = (int)domTreeDFSOrder.size() - 1; i >= 0; i--) {
        BasicBlock *current = domTreeDFSOrder[i];
        set<BasicBlock *> df;
        
        for (BasicBlock *dominee : current->getDominees()) {
            for (BasicBlock *dfe : dominee->getDominanceFrontier()) {
                if (current->getDominees().count(dfe) > 0) {
                    continue;
                }
                df.insert(dfe);
            }
        }
        
        for (BasicBlock *child : current->getChildren()) {
            if (child->getDominator() == current) {
                continue;
            }
            df.insert(child);
        }
        
        ssaProgram->setDomFrontier(current, df);
    }
}
