#include "JavaConstant.h"

JavaConstant::JavaConstant(JavaConstantTag t) : tag(t) {}

JavaConstant::~JavaConstant() {}

JavaConstantTag JavaConstant::getTag() {
    return tag;
}

JavaClassConstant::JavaClassConstant(ifstream& s) : JavaConstant(JavaConstantTag::Class) {
    name_index = get_u2(s);
}

JavaClassConstant::~JavaClassConstant() {}

JavaRefConstant::JavaRefConstant(JavaConstantTag t, ifstream& s) : JavaConstant(t) {
    class_index = get_u2(s);
    name_and_type_index = get_u2(s);
}

JavaRefConstant::~JavaRefConstant() {}

JavaFieldRefConstant::JavaFieldRefConstant(ifstream& s) : JavaRefConstant(JavaConstantTag::FieldRef, s) {}

JavaFieldRefConstant::~JavaFieldRefConstant() {}

JavaMethodRefConstant::JavaMethodRefConstant(ifstream& s) : JavaRefConstant(JavaConstantTag::MethodRef, s) {}

JavaMethodRefConstant::~JavaMethodRefConstant() {}

JavaInterfaceMethodRefConstant::JavaInterfaceMethodRefConstant(ifstream& s) : JavaRefConstant(JavaConstantTag::InterfaceMethodRef, s) {}

JavaInterfaceMethodRefConstant::~JavaInterfaceMethodRefConstant() {}

JavaStringConstant::JavaStringConstant(ifstream& s) : JavaConstant(JavaConstantTag::String) {
    string_index = get_u2(s);
}

JavaStringConstant::~JavaStringConstant() {}

JavaIntegerConstant::JavaIntegerConstant(ifstream& s) : JavaConstant(JavaConstantTag::Integer) {
    bytes = get_u4(s);
}

JavaIntegerConstant::~JavaIntegerConstant() {}

JavaLongConstant::JavaLongConstant(ifstream& s) : JavaConstant(JavaConstantTag::Long) {
    high_bytes = get_u4(s);
    low_bytes = get_u4(s);
}

JavaLongConstant::~JavaLongConstant() {}

JavaFloatConstant::JavaFloatConstant(ifstream& s) : JavaConstant(JavaConstantTag::Float) {
    bytes = get_u4(s);
}

JavaFloatConstant::~JavaFloatConstant() {}

JavaDoubleConstant::JavaDoubleConstant(ifstream& s) : JavaConstant(JavaConstantTag::Double) {
    high_bytes = get_u4(s);
    low_bytes = get_u4(s);
}

JavaDoubleConstant::~JavaDoubleConstant() {}

JavaNameAndTypeConstant::JavaNameAndTypeConstant(ifstream& s) : JavaConstant(JavaConstantTag::NameAndType) {
    name_index = get_u2(s);
    descriptor_index = get_u2(s);
}

JavaNameAndTypeConstant::~JavaNameAndTypeConstant() {}

JavaUtf8Constant::JavaUtf8Constant(ifstream& s) : JavaConstant(JavaConstantTag::Utf8) {
    length = get_u2(s);
    for (int i = 0; i < length; i++) {
        u1 byte = get_u1(s);
        bytes.push_back(byte);
    }
}

JavaUtf8Constant::~JavaUtf8Constant() {}

string JavaUtf8Constant::getString() {
    return string(bytes.begin(), bytes.end());
}

JavaMethodHandleConstant::JavaMethodHandleConstant(ifstream& s) : JavaConstant(JavaConstantTag::MethodHandle) {
    reference_kind = get_u1(s);
    reference_index = get_u2(s);
}

JavaMethodHandleConstant::~JavaMethodHandleConstant() {}

JavaMethodTypeConstant::JavaMethodTypeConstant(ifstream& s) : JavaConstant(JavaConstantTag::MethodType) {
    descriptor_index = get_u2(s);
}

JavaMethodTypeConstant::~JavaMethodTypeConstant() {}

JavaDynamicConstant::JavaDynamicConstant(ifstream& s) : JavaConstant(JavaConstantTag::Dynamic) {
    bootstrap_method_attr_index = get_u2(s);
    name_and_type_index = get_u2(s);
}

JavaDynamicConstant::~JavaDynamicConstant() {}

JavaInvokeDynamicConstant::JavaInvokeDynamicConstant(ifstream& s) : JavaConstant(JavaConstantTag::InvokeDynamic) {
    bootstrap_method_attr_index = get_u2(s);
    name_and_type_index = get_u2(s);
}

JavaInvokeDynamicConstant::~JavaInvokeDynamicConstant() {}

JavaModuleConstant::JavaModuleConstant(ifstream& s) : JavaConstant(JavaConstantTag::Module) {
    name_index = get_u2(s);
}

JavaModuleConstant::~JavaModuleConstant() {}

JavaPackageConstant::JavaPackageConstant(ifstream& s) : JavaConstant(JavaConstantTag::Package) {
    name_index = get_u2(s);
}

JavaPackageConstant::~JavaPackageConstant() {}
