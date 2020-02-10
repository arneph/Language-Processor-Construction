#ifndef JavaClass_h
#define JavaClass_h

#include <map>
#include <string>

#include "Base.h"
#include "JavaAttribute.h"
#include "JavaConstantsPool.h"
#include "JavaConstant.h"
#include "JavaField.h"
#include "JavaMethod.h"

using namespace std;

class JavaClass {
    u2 major_version;
    u2 minor_version;
    
    u2 acceess_flags;
    u2 this_class;
    u2 super_class;
    
    JavaConstantsPool *consts_pool;
    
    vector<u2> interfaces;
    
    vector<JavaField *> fields;
    map<string, int> field_names;
    
    vector<JavaMethod *> methods;
    map<string, int> method_names;
    
    vector<JavaAttribute *> attributes;
    map<string, int> attribute_names;
    
public:
    JavaClass(ifstream& stream);
    ~JavaClass();
    
    u2 getMajorVersion();
    u2 getMinorVersion();
    
    JavaConstantsPool* getConstantsPool();
    
    JavaMethod* getMainMethod();
};

#endif /* class_h */
