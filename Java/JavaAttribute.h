#ifndef JavaAttribute_h
#define JavaAttribute_h

#include <map>
#include <vector>

#include "Base.h"

using namespace std;

class JavaAttribute {
    u2 name_index;
    vector<u1> info;
protected:
    JavaAttribute(JavaAttribute *attr);
public:
    JavaAttribute(ifstream& stream);
    ~JavaAttribute();
    
    u2 getNameIndex();
    const vector<u1> getInfo();
};

class JavaCodeAttribute : public JavaAttribute {
    u2 max_stack;
    u2 max_locals;
    
    vector<u1> code;
    
    vector<JavaAttribute *> attributes;
    map<string, int> attribute_names;
public:
    JavaCodeAttribute(JavaAttribute *attr);
    ~JavaCodeAttribute();
    
    u2 getMaxStack();
    u2 getMaxLocals();
    
    const vector<u1> getCode();
};

#endif /* JavaAttribute_h */
