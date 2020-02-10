#include "MachineCodeGenerator.h"

MachineCodeGenerator::MachineCodeGenerator(Program *pgm) {
    program = pgm;
    
    generateCode();
    patchCode();
}

MachineCodeGenerator::~MachineCodeGenerator() {}

Program * MachineCodeGenerator::getProgram() {
    return program;
}

vector<uint8_t> MachineCodeGenerator::getFinalCode() {
    return finalCode;
}

string MachineCodeGenerator::getBlockText() {
    stringstream ss;
    
    for (int i = 0; i < program->getBlocks().size(); i++) {
        BasicBlock *block = program->getBlocks()[i];
        
        ss << "lbl_" << (char)('A' + block->getNumber()) << ":" << endl;
        ss << blockText[block];
        ss << endl;
    }
    
    return ss.str();
}

string rstr(int r) {
    if (r == 0) {
        return "rax";
    }else if (r == 1) {
        return "rcx";
    }else if (r == 2) {
        return "rdx";
    }else if (r == 3) {
        return "rbx";
        
    }else if (r == 4) {
        return "rsp";
    }else if (r == 5) {
        return "rbp";
    }else if (r == 6) {
        return "rsi";
    }else if (r == 7) {
        return "rdi";
    }
    return "r??";
}

void MachineCodeGenerator::generateCode() {
    for (int i = 0; i < program->getBlocks().size(); i++) {
        BasicBlock *block = program->getBlocks()[i];
        stringstream ss;
        
        if (i == 0) {
            blockCode[block].push_back(0x55);
            
            blockCode[block].push_back(0x48);
            blockCode[block].push_back(0x89);
            blockCode[block].push_back(0xE5);
            
            blockCode[block].push_back(0x48);
            blockCode[block].push_back(0x83);
            blockCode[block].push_back(0xEC);
            blockCode[block].push_back(0x30);
            
            blockCode[block].push_back(0x48);
            blockCode[block].push_back(0x89);
            blockCode[block].push_back(0x5D);
            blockCode[block].push_back(0xF8);
        }
        
        for (Instruction *instr : block->getInstructions()) {
            switch (instr->getOperator()) {
                case Add:{
                    int r0 =
                        ((RegisterValue *) instr->getArguments()[0])
                        ->getRegisterNumber();
                    Value *argA = instr->getArguments()[1];
                    Value *argB = instr->getArguments()[2];
                    bool hasConst = false;
                    int c = 0;
                    int r1 = -1;
                    int r2 = -1;
                    if (ConstantValue *cv =
                        dynamic_cast<ConstantValue *>(argA)) {
                        hasConst = true;
                        c = cv->getValue();
                        r1 = ((RegisterValue *)argB)->getRegisterNumber();
                    }else if (ConstantValue *cv =
                              dynamic_cast<ConstantValue *>(argB)) {
                        hasConst = true;
                        c = cv->getValue();
                        r1 = ((RegisterValue *)argA)->getRegisterNumber();
                    }else{
                        hasConst = false;
                        r1 = ((RegisterValue *)argA)->getRegisterNumber();
                        r2 = ((RegisterValue *)argB)->getRegisterNumber();
                    }
                    
                    if (!hasConst && r0 != r1 && r0 == r2) {
                        r2 = r1; r1 = r0;
                    }
                    
                    if (r0 != r1) {
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0x89);
                        blockCode[block].push_back(0xC0 + r0+r1*8);
                        
                        ss << "MOV ";
                        ss << rstr(r0);
                        ss << ", ";
                        ss << rstr(r1);
                        ss << endl;
                    }
                    
                    if (hasConst) {
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0x83);
                        blockCode[block].push_back(0xC0 + r0);
                        blockCode[block].push_back((uint8_t) c);
                        
                        ss << "ADD ";
                        ss << rstr(r0);
                        ss << ", ";
                        ss << "0x" << hex << c;
                        ss << endl;
                    }else{
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0x01);
                        blockCode[block].push_back(0xC0 + r0+r2*8);
                        
                        ss << "ADD ";
                        ss << rstr(r0);
                        ss << ", ";
                        ss << rstr(r2);
                        ss << endl;
                    }
                    
                    break;
                }
                case Subtract:
                case Compare:{
                    int r0 =
                    ((RegisterValue *) instr->getArguments()[0])
                    ->getRegisterNumber();
                    Value *argA = instr->getArguments()[1];
                    Value *argB = instr->getArguments()[2];
                    bool arg1const = false;
                    bool arg2const = false;
                    int c1 = 0;
                    int c2 = 0;
                    int r1 = -1;
                    int r2 = -1;
                    if (ConstantValue *cv =
                        dynamic_cast<ConstantValue *>(argA)) {
                        arg1const = true;
                        c1 = cv->getValue();
                    }else{
                        arg1const = false;
                        r1 = ((RegisterValue *)argA)->getRegisterNumber();
                    }
                    if (ConstantValue *cv =
                        dynamic_cast<ConstantValue *>(argB)) {
                        arg2const = true;
                        c2 = cv->getValue();
                    }else{
                        arg2const = false;
                        r2 = ((RegisterValue *)argB)->getRegisterNumber();
                    }
                    
                    if (arg1const) {
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0xC7);
                        blockCode[block].push_back(0xC0 + r0);
                        blockCode[block].push_back(0x00 + c1);
                        blockCode[block].push_back(0x00);
                        blockCode[block].push_back(0x00);
                        blockCode[block].push_back(0x00);
                        
                        ss << "MOV ";
                        ss << rstr(r0);
                        ss << ", ";
                        ss << "0x" << hex << c1;
                        ss << endl;
                        
                    }else if (r0 != r1) {
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0x89);
                        blockCode[block].push_back(0xC0 + r0+r1*8);
                        
                        ss << "MOV ";
                        ss << rstr(r0);
                        ss << ", ";
                        ss << rstr(r1);
                        ss << endl;
                    }
                    
                    if (arg2const) {
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0x83);
                        if (instr->getOperator() == Subtract)
                            blockCode[block].push_back(0xE8 + r0);
                        else if (instr->getOperator() == Compare)
                            blockCode[block].push_back(0xF8 + r0);
                        blockCode[block].push_back((uint8_t) c2);
                        
                        if (instr->getOperator() == Subtract)
                            ss << "SUB ";
                        else if (instr->getOperator() == Compare)
                            ss << "CMP ";
                        ss << rstr(r0);
                        ss << ", ";
                        ss << "0x" << hex << c2;
                        ss << endl;
                    }else{
                        blockCode[block].push_back(0x48);
                        if (instr->getOperator() == Subtract)
                            blockCode[block].push_back(0x29);
                        else if (instr->getOperator() == Compare)
                            blockCode[block].push_back(0x39);
                        blockCode[block].push_back(0xC0 + r0+r2*8);
                        
                        if (instr->getOperator() == Subtract)
                            ss << "SUB ";
                        else if (instr->getOperator() == Compare)
                            ss << "CMP ";
                        ss << rstr(r0);
                        ss << ", ";
                        ss << rstr(r2);
                        ss << endl;
                    }
                    
                    break;
                }
                case Branch: {
                    int destBlockNum =
                        ((InstructionAddressValue *)instr->getArguments()[0])
                        ->getBlockNumber();
                    
                    blockCode[block].push_back(0xEB);
                    blockCode[block].push_back(0x00);
                    
                    int l = (int)blockCode[block].size() - 1;
                    
                    ss << "JMP ";
                    ss << "lbl_" << (char)('A' + destBlockNum);
                    ss << endl;
                    
                    patchIndices[block].push_back(l);
                    patchDestinations[block].push_back(destBlockNum);
                    
                    break;
                }
                case BranchNotEqual:
                case BranchEqual:
                case BranchGreaterThan:
                case BranchGreaterThanOrEqual:
                case BranchLessThanOrEqual:
                case BranchLessThan: {
                    int destBlockNumA =
                        ((InstructionAddressValue *)instr->getArguments()[1])
                        ->getBlockNumber();
                    int destBlockNumB =
                        ((InstructionAddressValue *)instr->getArguments()[2])
                        ->getBlockNumber();
                    
                    if (instr->getOperator() == BranchNotEqual)
                        blockCode[block].push_back(0x75);
                    else if (instr->getOperator() == BranchEqual)
                        blockCode[block].push_back(0x74);
                    else if (instr->getOperator() == BranchGreaterThan)
                        blockCode[block].push_back(0x7F);
                    else if (instr->getOperator() == BranchGreaterThanOrEqual)
                        blockCode[block].push_back(0x7D);
                    else if (instr->getOperator() == BranchLessThanOrEqual)
                        blockCode[block].push_back(0x7E);
                    else if (instr->getOperator() == BranchLessThan)
                        blockCode[block].push_back(0x7C);
                    blockCode[block].push_back(0x00);
                    
                    int l1 = (int)blockCode[block].size() - 1;
                    
                    
                    if (instr->getOperator() == BranchNotEqual)
                        ss << "JNE ";
                    else if (instr->getOperator() == BranchEqual)
                        ss << "JE  ";
                    else if (instr->getOperator() == BranchGreaterThan)
                        ss << "JG  ";
                    else if (instr->getOperator() == BranchGreaterThanOrEqual)
                        ss << "JGE ";
                    else if (instr->getOperator() == BranchLessThanOrEqual)
                        ss << "JLE ";
                    else if (instr->getOperator() == BranchLessThan)
                        ss << "JL  ";
                    ss << "lbl_" << (char)('A' + destBlockNumA);
                    ss << endl;
                    
                    patchIndices[block].push_back(l1);
                    patchDestinations[block].push_back(destBlockNumA);
                    
                    blockCode[block].push_back(0xEB);
                    blockCode[block].push_back(0x00);
                    
                    int l2 = (int)blockCode[block].size() - 1;
                    
                    ss << "JMP ";
                    ss << "lbl_" << (char)('A' + destBlockNumB);
                    ss << endl;
                    
                    patchIndices[block].push_back(l2);
                    patchDestinations[block].push_back(destBlockNumB);
                    
                    break;
                }
                case End: {
                    blockCode[block].push_back(0x48);
                    blockCode[block].push_back(0x8B);
                    blockCode[block].push_back(0x5D);
                    blockCode[block].push_back(0xF8);
                    
                    blockCode[block].push_back(0x48);
                    blockCode[block].push_back(0x83);
                    blockCode[block].push_back(0xC4);
                    blockCode[block].push_back(0x30);
                    
                    blockCode[block].push_back(0x5D);
                    
                    blockCode[block].push_back(0xC3);
                    ss << "RET" << endl;
                    
                    break;
                }
                case Move:{
                    int r0 =
                        ((RegisterValue *) instr->getArguments()[0])
                        ->getRegisterNumber();
                    Value *origValue = instr->getArguments()[1];
                    bool hasConst = false;
                    int c = 0;
                    int r1 = -1;
                    if (ConstantValue *cv = dynamic_cast<ConstantValue *>(origValue)) {
                        hasConst = true;
                        c = cv->getValue();
                    }else{
                        hasConst = false;
                        r1 = ((RegisterValue *) origValue)->getRegisterNumber();
                    }
                    
                    if (hasConst) {
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0xC7);
                        blockCode[block].push_back(0xC0 + r0);
                        blockCode[block].push_back(0x00 + c);
                        blockCode[block].push_back(0x00);
                        blockCode[block].push_back(0x00);
                        blockCode[block].push_back(0x00);
                        
                        ss << "MOV ";
                        ss << rstr(r0);
                        ss << ", ";
                        ss << "0x" << hex << c;
                        ss << endl;
                        
                    }else{
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0x89);
                        blockCode[block].push_back(0xC0 + r0+r1*8);
                        
                        ss << "MOV ";
                        ss << rstr(r0);
                        ss << ", ";
                        ss << rstr(r1);
                        ss << endl;
                    }
                    
                    break;
                }
                case ProcedureCall:{
                    Value *origValue = instr->getArguments()[0];
                    bool hasConst = false;
                    int c = 0;
                    int r1 = -1;
                    if (ConstantValue *cv = dynamic_cast<ConstantValue *>(origValue)) {
                        hasConst = true;
                        c = cv->getValue();
                    }else{
                        hasConst = false;
                        r1 = ((RegisterValue *) origValue)->getRegisterNumber();
                    }
                    
                    if (hasConst) {
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0xC7);
                        blockCode[block].push_back(0xC7);
                        blockCode[block].push_back(0x00 + c);
                        blockCode[block].push_back(0x00);
                        blockCode[block].push_back(0x00);
                        blockCode[block].push_back(0x00);
                        
                        ss << "MOV rdi, ";
                        ss << "0x" << hex << c;
                        ss << endl;
                        
                    }else{
                        blockCode[block].push_back(0x48);
                        blockCode[block].push_back(0x89);
                        blockCode[block].push_back(0xC7+r1*8);
                        
                        ss << "MOV rdi, ";
                        ss << rstr(r1);
                        ss << endl;
                    }
                    
                    blockCode[block].push_back(0xE8);
                    blockCode[block].push_back(0x00);
                    blockCode[block].push_back(0x00);
                    blockCode[block].push_back(0x00);
                    blockCode[block].push_back(0x00);
                    
                    int l = (int)blockCode[block].size() - 4;
                    
                    ss << "CALL printInt\n";
                    
                    printIndices[block].push_back(l);
                    
                }
                default:
                    break;
            }
        }
        
        blockText[block] = ss.str();
    }
}

void MachineCodeGenerator::patchCode() {
    for (int i = 0; i < program->getBlocks().size(); i++) {
        BasicBlock *block = program->getBlocks()[i];
        
        blockStarts[block] = (int)finalCode.size();
        
        for (uint8_t byte : blockCode[block]) {
            finalCode.push_back(byte);
        }
    }
    
    for (int i = 0; i < program->getBlocks().size(); i++) {
        BasicBlock *block = program->getBlocks()[i];
        int blockStart = blockStarts[block];
        
        for (int j = 0; j < patchIndices[block].size(); j++) {
            int index = patchIndices[block][j];
            int destBlockNum = patchDestinations[block][j];
            int destBlockStart = blockStarts[program->getBlocks()[destBlockNum]];
            
            int orig = blockStart + index + 1;
            int dest = destBlockStart;
            int offset = dest - orig;
            
            finalCode[blockStart + index] = (uint8_t) offset;
        }
    }
}

void MachineCodeGenerator::patchPrintInt(void *codeBaseAddr, void *printIntAddr) {
    for (int i = 0; i < program->getBlocks().size(); i++) {
        BasicBlock *block = program->getBlocks()[i];
        int blockStart = blockStarts[block];
        
        for (int j = 0; j < printIndices[block].size(); j++) {
            int index = printIndices[block][j];
            
            int64_t orig = (int64_t)codeBaseAddr + blockStart + index + 4;
            int64_t dest = (int64_t)printIntAddr;
            int64_t offset = dest - orig;
            
            finalCode[blockStart + index + 0] = (uint8_t) ((offset >>  0) & 0xFF);
            finalCode[blockStart + index + 1] = (uint8_t) ((offset >>  8) & 0xFF);
            finalCode[blockStart + index + 2] = (uint8_t) ((offset >> 16) & 0xFF);
            finalCode[blockStart + index + 3] = (uint8_t) ((offset >> 24) & 0xFF);
        }
    }
}
