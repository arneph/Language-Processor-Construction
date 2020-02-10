#ifndef MachineCodeGenerator_h
#define MachineCodeGenerator_h

#include <map>
#include <sstream>
#include <unordered_map>

#include "../Intermediate Representation/IR.h"

class MachineCodeGenerator {
    Program *program;
    
    map<BasicBlock *, vector<uint8_t>> blockCode;
    map<BasicBlock *, string> blockText;
    
    unordered_map<BasicBlock *, vector<int>> patchIndices;
    unordered_map<BasicBlock *, vector<int>> patchDestinations;
    unordered_map<BasicBlock *, vector<int>> printIndices;
    
    unordered_map<BasicBlock *, int> blockStarts;
    vector<uint8_t> finalCode;
    
    void generateCode();
    void patchCode();
    
public:
    MachineCodeGenerator(Program *program);
    ~MachineCodeGenerator();
    
    Program * getProgram();
    
    void patchPrintInt(void *codeBaseAddr, void *printIntAddr);
    
    vector<uint8_t> getFinalCode();
    string getBlockText();
};

#endif /* MachineCodeGenerator_h */
