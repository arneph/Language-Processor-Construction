#ifndef MemoryAllocator_h
#define MemoryAllocator_h

#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../Intermediate Representation/IR.h"
#include "LiveRangeTable.h"

using namespace std;

class MemoryAllocator {
    Program *program;
    LiveRangeTable *liveRangeTable;
    
    unordered_map<
            Value *,
            unordered_set<Value *,
                decltype(ValueHash),
                decltype(ValueEqual)> *,
            decltype(ValueHash), decltype(ValueEqual)>
        graph =
        unordered_map<Value *,
            unordered_set<Value *,
                decltype(ValueHash),
                decltype(ValueEqual)> *,
            decltype(ValueHash),
            decltype(ValueEqual)>
            (0, ValueHash, ValueEqual);
    unordered_map<Value *, int,
            decltype(ValueHash),
            decltype(ValueEqual)>
        colors = unordered_map<Value *, int,
            decltype(ValueHash),
            decltype(ValueEqual)>
            (0, ValueHash, ValueEqual);
    
    stringstream valueAllocationTable;
    
    void buildInterferenceGraph();
    void colorInterferenceGraph();
    void applyInterferenceGraphColors();
    void resolvePhiInstructions();
    
    void addNode(Value *value);
    void addEdge(Value *a, Value *b);
    
public:
    MemoryAllocator(Program *program, LiveRangeTable *table);
    ~MemoryAllocator();
    
    VCGGraph * getInterferenceGraph();
    string getValueAllocationTable();
};

#endif /* MemoryAllocator_h */
