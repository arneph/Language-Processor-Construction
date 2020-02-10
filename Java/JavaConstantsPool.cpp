#include "JavaConstantsPool.h"

JavaConstantsPool::JavaConstantsPool(ifstream& s) {
    u2 c_consts_pool = get_u2(s);
    for (int i = 1; i < c_consts_pool; i++) {
        u1 tag = get_u1(s);
        switch (tag) {
            case JavaConstantTag::Utf8: {
                JavaUtf8Constant *utf8_const = new JavaUtf8Constant(s);
                string name = utf8_const->getString();
                
                if (name_consts.count(name)) {
                    cerr << "Duplicate UTF8 constant\n";
                }
                
                consts_pool.push_back(utf8_const);
                name_consts[name] = i - 1;
            }   break;
            case JavaConstantTag::Integer:
                consts_pool.push_back(new JavaIntegerConstant(s));
                break;
            case JavaConstantTag::Float:
                consts_pool.push_back(new JavaFloatConstant(s));
                break;
            case JavaConstantTag::Long:
                consts_pool.push_back(new JavaLongConstant(s));
                break;
            case JavaConstantTag::Double:
                consts_pool.push_back(new JavaDoubleConstant(s));
                break;
            case JavaConstantTag::Class:
                consts_pool.push_back(new JavaClassConstant(s));
                break;
            case JavaConstantTag::String:
                consts_pool.push_back(new JavaStringConstant(s));
                break;
            case JavaConstantTag::FieldRef:
                consts_pool.push_back(new JavaFieldRefConstant(s));
                break;
            case JavaConstantTag::MethodRef:
                consts_pool.push_back(new JavaMethodRefConstant(s));
                break;
            case JavaConstantTag::InterfaceMethodRef:
                consts_pool.push_back(new JavaInterfaceMethodRefConstant(s));
                break;
            case JavaConstantTag::NameAndType:
                consts_pool.push_back(new JavaNameAndTypeConstant(s));
                break;
            case JavaConstantTag::MethodHandle:
                consts_pool.push_back(new JavaMethodHandleConstant(s));
                break;
            case JavaConstantTag::MethodType:
                consts_pool.push_back(new JavaMethodTypeConstant(s));
                break;
            case JavaConstantTag::Dynamic:
                consts_pool.push_back(new JavaDynamicConstant(s));
                break;
            case JavaConstantTag::InvokeDynamic:
                consts_pool.push_back(new JavaInvokeDynamicConstant(s));
                break;
            case JavaConstantTag::Module:
                consts_pool.push_back(new JavaModuleConstant(s));
                break;
            case JavaConstantTag::Package:
                consts_pool.push_back(new JavaPackageConstant(s));
                break;
                
            default:
                cerr << "const tag wrong\n";
                return;
        }
    }
}

JavaConstantsPool::~JavaConstantsPool() {}

JavaConstant* JavaConstantsPool::getConstantAtIndex(int index) {
    if (index <= 0 || index > consts_pool.size()) {
        cerr << "invalid constant index\n";
    }
    
    return consts_pool[index - 1];
}

int JavaConstantsPool::getIndexOfNameConstant(string name) {
    if (name_consts.count(name) == 0) {
        return -1;
    }
    
    return name_consts[name];
}
