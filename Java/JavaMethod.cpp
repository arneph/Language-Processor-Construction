#include "JavaMethod.h"

JavaMethod::JavaMethod(JavaConstantsPool *pool, ifstream& s) {
    consts_pool = pool;
    
    access_flags = get_u2(s);
    name_index = get_u2(s);
    descriptor_index = get_u2(s);
    
    u2 c_attributes = get_u2(s);
    for (int i = 0; i < c_attributes; i++) {
        JavaAttribute *attribute = new JavaAttribute(s);
        
        int name_index = attribute->getNameIndex();
        string name;
        
        JavaUtf8Constant *utf8_const = static_cast<JavaUtf8Constant*> (consts_pool->getConstantAtIndex(name_index));
        name = utf8_const->getString();
        
        attributes.push_back(attribute);
        attribute_names[name] = i;
    }
    
    int code_index = attribute_names["Code"];
    code = new JavaCodeAttribute(attributes[code_index]);
}

JavaMethod::~JavaMethod() {}

u2 JavaMethod::getAccessFlags() {
    return access_flags;
}

u2 JavaMethod::getNameIndex() {
    return name_index;
}

u2 JavaMethod::getDescriptorIndex() {
    return descriptor_index;
}

string JavaMethod::getName() {
    JavaUtf8Constant *utf8_const = static_cast<JavaUtf8Constant*> (consts_pool->getConstantAtIndex(name_index));
    
    return utf8_const->getString();
}

const vector<JavaAttribute *> JavaMethod::getAttributes() {
    return attributes;
}

JavaCodeAttribute* JavaMethod::getCodeAttribute() {
    return code;
}
