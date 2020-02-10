#include "LiveRangeTable.h"

LiveRangeTable::LiveRangeTable() {}
LiveRangeTable::~LiveRangeTable() {}

string LiveRangeTable::toString() const {
    stringstream ss;
    ss << "Value     | Def | Uses\n";
    ss << "----------|-----|-------------------------------------\n";
    for (auto it = definitions.begin(); it != definitions.end(); ++it) {
        ss << setw(9) << setfill(' ') << it->first->toString();
        ss << " | ";
        ss << setw(3) << setfill('0') << it->second;
        ss << " | ";
        bool first = true;
        for (int use : uses.at(it->first)) {
            if (!first) ss << ", ";
            else first = false;
            ss << setw(3) << setfill('0') << use;
        }
        ss << endl;
    }
    return ss.str();
}

int LiveRangeTable::getDefinition(Value * value) {
    return definitions[value];
}

void LiveRangeTable::setDefinition(Value *value, int instrNumber) {
    definitions[value] = instrNumber;
    if (uses.count(value) == 0) {
        uses[value] = unordered_set<int>();
    }
}

int LiveRangeTable::getUseCount(Value *value) {
    return (int)uses[value].size();
}

const unordered_set<int>& LiveRangeTable::getUses(Value *value) {
    return uses[value];
}

void LiveRangeTable::addUse(Value *value, int instrNumber) {
    uses[value].insert(instrNumber);
    if (definitions.count(value) == 0) {
        definitions[value] = -1;
    }
}

void LiveRangeTable::removeUse(Value *value, int instrNumber) {
    auto it = uses[value].find(instrNumber);
    if (it != uses[value].end())
        uses[value].erase(it);
}

void LiveRangeTable::removeValue(Value *value) {
    auto itA = definitions.find(value);
    if (itA != definitions.end())
        definitions.erase(itA);
    auto itB = uses.find(value);
    if (itB != uses.end())
        uses.erase(itB);
}

Value * LiveRangeTable::getDefinedValue(int instrNumber) {
    for (auto it = definitions.begin();
         it != definitions.end(); ++it) {
        if (it->second == instrNumber) {
            return it->first;
        }
    }
    
    return nullptr;
}

const unordered_set<Value *> LiveRangeTable::getUsedValues(int instrNumber) {
    unordered_set<Value *, decltype(ValueHash), decltype(ValueEqual)>values(0, ValueHash, ValueEqual);
    
    for (auto it = uses.begin();
         it != uses.end(); ++it) {
        if (it->second.count(instrNumber)) {
            values.insert(it->first);
        }
    }
    
    unordered_set<Value *>results;
    for (Value *value : values) {
        results.insert(value);
    }
    
    return results;
}
