#include "JavaClass.h"

JavaClass::JavaClass(ifstream& s) {
    u4 magic = get_u4(s);
    if (magic != 0xcafebabe) {
        cerr << "magic wrong\n";
        return;
    }
    
    minor_version = get_u2(s);
    major_version = get_u2(s);
    
    consts_pool = new JavaConstantsPool(s);
    
    acceess_flags = get_u2(s);
    this_class = get_u2(s);
    super_class = get_u2(s);
    
    u2 c_interfaces = get_u2(s);
    for (int i = 0; i < c_interfaces; i++) {
        interfaces.push_back(get_u2(s));
    }
    
    u2 c_fields = get_u2(s);
    for (int i = 0; i < c_fields; i++) {
        JavaField *field = new JavaField(consts_pool, s);
        
        fields.push_back(field);
        field_names[field->getName()] = i;
    }
    
    u2 c_methods = get_u2(s);
    for (int i = 0; i < c_methods; i++) {
        JavaMethod *method = new JavaMethod(consts_pool, s);
        
        methods.push_back(method);
        method_names[method->getName()] = i;
    }
    
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

JavaClass::~JavaClass() {}

u2 JavaClass::getMajorVersion() {
    return major_version;
}

u2 JavaClass::getMinorVersion() {
    return minor_version;
}

JavaConstantsPool* JavaClass::getConstantsPool() {
    return consts_pool;
}

JavaMethod* JavaClass::getMainMethod() {
    int index = method_names["main"];
    
    return methods[index];
}
