#include "JavaAttribute.h"

JavaAttribute::JavaAttribute(JavaAttribute *attr) {
    name_index = attr->name_index;
    info = attr->info;
}

JavaAttribute::JavaAttribute(ifstream& s) {
    name_index = get_u2(s);
    
    u4 length = get_u4(s);
    for (int i = 0; i < length; i++) {
        info.push_back(get_u1(s));
    }
}

JavaAttribute::~JavaAttribute() {}

u2 JavaAttribute::getNameIndex() {
    return name_index;
}

const vector<u1> JavaAttribute::getInfo() {
    return info;
}

JavaCodeAttribute::JavaCodeAttribute(JavaAttribute *attr) : JavaAttribute(attr) {
    vector<u1> info = attr->getInfo();
    u1 t1, t2, t3, t4;
    
    t1 = info[0];
    t2 = info[1];
    max_stack = ((u2) t1) << 8 | ((u2) t2) << 0;
    
    t1 = info[2];
    t2 = info[3];
    max_locals = ((u2) t1) << 8 | ((u2) t2) << 0;
    
    t1 = info[4];
    t2 = info[5];
    t3 = info[6];
    t4 = info[7];
    u4 c_code = ((u4) t1) << 24 | ((u4) t2) << 16 |
                ((u4) t3) <<  8 | ((u4) t4) <<  0;
    code = vector<u1>(info.begin() + 8, info.begin() + 8 + c_code);
    
    /*
    t1 = info[8 + c_code * 1 + 0];
    t2 = info[8 + c_code * 1 + 1];
    u2 c_exceptions = ((u2) t1) << 8 | ((u2) t2) << 0;
    
    t1 = info[10 + c_code + c_exceptions * 8 + 0];
    t2 = info[10 + c_code + c_exceptions * 8 + 1];
    u2 c_attributes = ((u2) t1) << 8 | ((u2) t2) << 0;
     */
}

u2 JavaCodeAttribute::getMaxStack() {
    return max_stack;
}

u2 JavaCodeAttribute::getMaxLocals() {
    return max_locals;
}

const vector<u1> JavaCodeAttribute::getCode() {
    return code;
}
