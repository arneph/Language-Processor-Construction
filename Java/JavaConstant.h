#ifndef JavaConstant_h
#define JavaConstant_h

#include <vector>

#include "Base.h"

using namespace std;

typedef enum JavaConstantTag : u1 {
    Utf8 = 1,
    Integer = 3,
    Float = 4,
    Long = 5,
    Double = 6,
    Class = 7,
    String = 8,
    FieldRef = 9,
    MethodRef = 10,
    InterfaceMethodRef = 11,
    NameAndType = 12,
    MethodHandle = 15,
    MethodType = 16,
    Dynamic = 17,
    InvokeDynamic = 18,
    Module = 19,
    Package = 20
} JavaConstantTag;

class JavaConstant {
    JavaConstantTag tag;
protected:
    JavaConstant(JavaConstantTag tag);
public:
    ~JavaConstant();
    
    JavaConstantTag getTag();
};

class JavaClassConstant : public JavaConstant {
    u2 name_index;
public:
    JavaClassConstant(ifstream& stream);
    ~JavaClassConstant();
};

class JavaRefConstant : public JavaConstant {
    u2 class_index;
    u2 name_and_type_index;
protected:
    JavaRefConstant(JavaConstantTag tag, ifstream& stream);
    ~JavaRefConstant();
};

class JavaFieldRefConstant : public JavaRefConstant {
public:
    JavaFieldRefConstant(ifstream& stream);
    ~JavaFieldRefConstant();
};

class JavaMethodRefConstant : public JavaRefConstant {
public:
    JavaMethodRefConstant(ifstream& stream);
    ~JavaMethodRefConstant();
};

class JavaInterfaceMethodRefConstant : public JavaRefConstant {
public:
    JavaInterfaceMethodRefConstant(ifstream& stream);
    ~JavaInterfaceMethodRefConstant();
};

class JavaStringConstant : public JavaConstant {
    u2 string_index;
public:
    JavaStringConstant(ifstream& stream);
    ~JavaStringConstant();
};

class JavaIntegerConstant : public JavaConstant {
    u4 bytes;
public:
    JavaIntegerConstant(ifstream& stream);
    ~JavaIntegerConstant();
};

class JavaLongConstant : public JavaConstant {
    u4 high_bytes;
    u4 low_bytes;
public:
    JavaLongConstant(ifstream& stream);
    ~JavaLongConstant();
};

class JavaFloatConstant : public JavaConstant {
    u4 bytes;
public:
    JavaFloatConstant(ifstream& stream);
    ~JavaFloatConstant();
};

class JavaDoubleConstant : public JavaConstant {
    u4 high_bytes;
    u4 low_bytes;
public:
    JavaDoubleConstant(ifstream& stream);
    ~JavaDoubleConstant();
};

class JavaNameAndTypeConstant : public JavaConstant {
    u2 name_index;
    u2 descriptor_index;
public:
    JavaNameAndTypeConstant(ifstream& stream);
    ~JavaNameAndTypeConstant();
};

class JavaUtf8Constant : public JavaConstant {
    u2 length;
    vector<u1> bytes;
public:
    JavaUtf8Constant(ifstream& stream);
    ~JavaUtf8Constant();
    
    string getString();
};

class JavaMethodHandleConstant : public JavaConstant {
    u1 reference_kind;
    u2 reference_index;
public:
    JavaMethodHandleConstant(ifstream& stream);
    ~JavaMethodHandleConstant();
};

class JavaMethodTypeConstant : public JavaConstant {
    u2 descriptor_index;
public:
    JavaMethodTypeConstant(ifstream& stream);
    ~JavaMethodTypeConstant();
};

class JavaDynamicConstant : public JavaConstant {
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
public:
    JavaDynamicConstant(ifstream& stream);
    ~JavaDynamicConstant();
};

class JavaInvokeDynamicConstant : public JavaConstant {
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
public:
    JavaInvokeDynamicConstant(ifstream& stream);
    ~JavaInvokeDynamicConstant();
};

class JavaModuleConstant : public JavaConstant {
    u2 name_index;
public:
    JavaModuleConstant(ifstream& stream);
    ~JavaModuleConstant();
};

class JavaPackageConstant : public JavaConstant {
    u2 name_index;
public:
    JavaPackageConstant(ifstream& stream);
    ~JavaPackageConstant();
};

#endif /* JavaConstant_h */
