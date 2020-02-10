#ifndef SSAGenerator_h
#define SSAGenerator_h

#include <set>
#include <unordered_map>
#include <vector>

#include "../Intermediate Representation/IR.h"

using namespace std;

class SSAGenerator {
    Program *program;
    
    void generateSSA();
    
public:
    SSAGenerator(Program *pgm);
    ~SSAGenerator();
    
    Program * getProgram();
};

#endif /* SSAGenerator_h */
