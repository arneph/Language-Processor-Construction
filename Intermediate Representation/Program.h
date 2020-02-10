#ifndef Program_h
#define Program_h

#include <set>
#include <vector>

#include "../VCG/VCG.h"

#include "BasicBlock.h"

using namespace std;

class Program {
    vector<BasicBlock *> blocks;
    
public:
    Program();
    ~Program();
    
    string toString();
    VCGGraph * getCFG(bool detailed);
    VCGGraph * getDomTree();
    string getDomFrontiersDescription();
    
    const vector<BasicBlock *>& getBlocks();
    BasicBlock * getStartBlock();
    BasicBlock * addBlock();
    
    void addCFGEdge(BasicBlock *parent, BasicBlock *child);
    void removeCFGEdge(BasicBlock *parent, BasicBlock *child);
    void addDomTreeEdge(BasicBlock *dominator, BasicBlock *dominee);
    void setDomFrontier(BasicBlock *dominator, set<BasicBlock *>& frontier);
};

#endif /* Program_h */
