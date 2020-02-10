#ifndef JavaMethod_h
#define JavaMethod_h

#include <map>
#include <vector>

#include "Base.h"
#include "JavaAttribute.h"
#include "JavaConstantsPool.h"
#include "JavaConstant.h"

using namespace std;

class JavaMethod {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    
    JavaConstantsPool *consts_pool;
    
    vector<JavaAttribute *> attributes;
    map<string, int> attribute_names;
    
    JavaCodeAttribute *code;
    
public:
    JavaMethod(JavaConstantsPool *pool, ifstream& stream);
    ~JavaMethod();
    
    u2 getAccessFlags();
    u2 getNameIndex();
    u2 getDescriptorIndex();
    
    string getName();
    
    const vector <JavaAttribute *> getAttributes();
    JavaCodeAttribute* getCodeAttribute();
};

#endif /* JavaMethod_h */
