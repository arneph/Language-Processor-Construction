#ifndef LiveRangeTable_h
#define LiveRangeTable_h

#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../Intermediate Representation/IR.h"

using namespace std;

class LiveRangeTable {
    unordered_map<Value *, int,
        decltype(ValueHash), decltype(ValueEqual)> 
        definitions =
        unordered_map<Value *, int,
            decltype(ValueHash), decltype(ValueEqual)>
            (0, ValueHash, ValueEqual);
    unordered_map<Value *, unordered_set<int>,
        decltype(ValueHash), decltype(ValueEqual)>
        uses =
        unordered_map<Value *, unordered_set<int>,
            decltype(ValueHash), decltype(ValueEqual)>
            (0, ValueHash, ValueEqual);
    
public:
    LiveRangeTable();
    ~LiveRangeTable();
    
    string toString() const;
    
    int getDefinition(Value *value);
    void setDefinition(Value *value, int instrNumber);
    
    int getUseCount(Value *value);
    const unordered_set<int>& getUses(Value *value);
    void addUse(Value *value, int instrNumber);
    void removeUse(Value *value, int instrNumber);
    
    void removeValue(Value *value);
    
    Value * getDefinedValue(int instrNumber);
    const unordered_set<Value *> getUsedValues(int instrNumber);
};

#endif /* LiveRangeTable_h */
