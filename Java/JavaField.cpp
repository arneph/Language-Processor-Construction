#include "JavaField.h"

JavaField::JavaField(JavaConstantsPool *pool, ifstream& s) {
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
}

JavaField::~JavaField() {}

u2 JavaField::getAccessFlags() {
    return access_flags;
}

u2 JavaField::getNameIndex() {
    return name_index;
}

u2 JavaField::getDescriptorIndex() {
    return descriptor_index;
}

string JavaField::getName() {
    JavaUtf8Constant *utf8_const = static_cast<JavaUtf8Constant*> (consts_pool->getConstantAtIndex(name_index));
    
    return utf8_const->getString();
}

const vector<JavaAttribute *> JavaField::getAttributes() {
    return attributes;
}
