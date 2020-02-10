#ifndef SSAOptimizer_h
#define SSAOptimizer_h

#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Intermediate Representation/IR.h"
#include "LiveRangeTable.h"

using namespace std;

class SSAOptimizer {
    Program *program;
    LiveRangeTable *liveRangeTable;
    
    stringstream moveInstructionEliminationTrace;
    stringstream constantPropagationTrace;
    stringstream copyPropagationTrace;
    stringstream unusedInstructionEliminationTrace;
    
    void eliminateMoveInstructions();
    void propagateConstants();
    void propagateCopies();
    void findLiveRanges();
    void eliminateUnusedInstructions();
    
public:
    SSAOptimizer(Program *pgm);
    ~SSAOptimizer();
    
    Program * getProgram();
    LiveRangeTable * getLiveRangeTable();
    
    string getMoveInstructionElinimationTrace();
    string getConstantPropagationTrace();
    string getCopyPropagationTrace();
    string getUnusedInstructionEliminationTrace();
};

#endif /* SSAOptimizer_h */
