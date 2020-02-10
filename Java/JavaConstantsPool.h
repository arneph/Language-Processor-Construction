#ifndef JavaConstantsPool_hpp
#define JavaConstantsPool_hpp

#include <map>
#include <vector>

#include "Base.h"
#include "JavaConstant.h"

class JavaConstantsPool {
    vector<JavaConstant *> consts_pool;
    map<string, int> name_consts;
public:
    JavaConstantsPool(ifstream& stream);
    ~JavaConstantsPool();
    
    JavaConstant* getConstantAtIndex(int index);
    int getIndexOfNameConstant(string name);
};

#endif /* JavaConstantsPool_h */
