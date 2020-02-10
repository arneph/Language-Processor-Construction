#include "Interpreter.h"

Interpreter::Interpreter(JavaClass *c) {
    j_class = c;
    j_main = c->getMainMethod();
        
    running = false;
    terminating = false;
    terminated = false;
    interpret_thread = thread([this]{ interpret(); });
}

Interpreter::~Interpreter() {
    terminate();
    
    interpret_thread.join();
}

bool Interpreter::isRunning() {
    return running;
}

bool Interpreter::isTerminated() {
    return terminated;
}

void Interpreter::run() {
    running = true;
}

void Interpreter::pause() {
    running = false;
}

void Interpreter::terminate() {
    if (terminating) return;
    if (terminated) return;
    
    terminating = true;
}

bool Interpreter::isVerbose() {
    return verbose;
}

void Interpreter::setVerbose(bool v) {
    verbose = v;
}

void Interpreter::interpret() {
    while (!terminating) {
        JavaCodeAttribute *code_attr = j_main->getCodeAttribute();
        code = code_attr->getCode();
        pc = 0;
        
        locals.insert(locals.begin(), code_attr->getMaxLocals(), 0x00000000);
        
        while (pc < code.size()) {
            if (verbose)
                printInstruction();
            executeInstruction();
        }
        
        break;
    }
    terminated = true;
}

void Interpreter::printInstruction() {
    cout << "pc: ";
    cout << setfill('0') << setw(3) << dec;
    cout << static_cast<int>(pc);
    cout << " 0x";
    cout << setfill('0') << setw(2) << hex;
    cout << static_cast<int>(code[pc]);
    cout << " " << InstrTable[code[pc]].name;
    cout << dec << endl;
}

void Interpreter::executeInstruction() {
    u1 opcode = code[pc++];
    
    if (opcode == iconst_m1.code) {
        stack.push_back(0x11111111);
    }else if (opcode == iconst_0.code) {
        stack.push_back(0x00000000);
    }else if (opcode == iconst_1.code) {
        stack.push_back(0x00000001);
    }else if (opcode == iconst_2.code) {
        stack.push_back(0x00000002);
    }else if (opcode == iconst_3.code) {
        stack.push_back(0x00000003);
    }else if (opcode == iconst_4.code) {
        stack.push_back(0x00000004);
    }else if (opcode == iconst_5.code) {
        stack.push_back(0x00000005);
      
    }else if (opcode == iload.code) {
        u1 index = code[pc++];
        stack.push_back(locals[index]);
    }else if (opcode == iload_0.code) {
        stack.push_back(locals[0]);
    }else if (opcode == iload_1.code) {
        stack.push_back(locals[1]);
    }else if (opcode == iload_2.code) {
        stack.push_back(locals[2]);
    }else if (opcode == iload_3.code) {
        stack.push_back(locals[3]);
        
    }else if (opcode == istore.code) {
        u1 index = code[pc++];
        locals[index] = stack.back();
        stack.pop_back();
    }else if (opcode == istore_0.code) {
        locals[0] = stack.back();
        stack.pop_back();
    }else if (opcode == istore_1.code) {
        locals[1] = stack.back();
        stack.pop_back();
    }else if (opcode == istore_2.code) {
        locals[2] = stack.back();
        stack.pop_back();
    }else if (opcode == istore_3.code) {
        locals[3] = stack.back();
        stack.pop_back();
        
    }else if (opcode == iadd.code) {
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        stack.push_back(a + b);
    }else if (opcode == isub.code) {
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        stack.push_back(a - b);
    }else if (opcode == imul.code) {
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        stack.push_back(a * b);
    }else if (opcode == idiv.code) {
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        stack.push_back(a / b);
    }else if (opcode == ineg.code) {
        int a;
        a = stack.back();
        stack.pop_back();
        stack.push_back(-a);
    }else if (opcode == ishl.code) {
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        stack.push_back(a << b);
    }else if (opcode == ishr.code) {
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        stack.push_back(a >> b);
    }else if (opcode == iinc.code) {
        u1 index = code[pc++];
        u1 c_val = code[pc++];
        locals[index] += (signed char)c_val;
        
    }else if (opcode == ifne.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a;
        a = stack.back();
        stack.pop_back();
        if (a != 0) pc += -3 + branch;
    }else if (opcode == ifeq.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a;
        a = stack.back();
        stack.pop_back();
        if (a == 0) pc += -3 + branch;
    }else if (opcode == iflt.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a;
        a = stack.back();
        stack.pop_back();
        if (a < 0) pc += -3 + branch;
    }else if (opcode == ifle.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a;
        a = stack.back();
        stack.pop_back();
        if (a <= 0) pc += -3 + branch;
    }else if (opcode == ifge.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a;
        a = stack.back();
        stack.pop_back();
        if (a >= 0) pc += -3 + branch;
    }else if (opcode == ifgt.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a;
        a = stack.back();
        stack.pop_back();
        if (a > 0) pc += -3 + branch;
        
    }else if (opcode == if_icmpne.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        if (a != b) pc += -3 + branch;
    }else if (opcode == if_icmpeq.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        if (a == b) pc += -3 + branch;
    }else if (opcode == if_icmplt.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        if (a < b) pc += -3 + branch;
    }else if (opcode == if_icmple.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        if (a <= b) pc += -3 + branch;
    }else if (opcode == if_icmpge.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        if (a >= b) pc += -3 + branch;
    }else if (opcode == if_icmpgt.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        int a, b;
        b = stack.back();
        stack.pop_back();
        a = stack.back();
        stack.pop_back();
        if (a != b) pc += -3 + branch;
        
    }else if (opcode == goto_.code) {
        u1 branch_b1 = code[pc++];
        u1 branch_b2 = code[pc++];
        short branch = ((u2) branch_b1) << 8 | ((u2) branch_b2 << 0);
        pc += -3 + branch;
        
    }else if (opcode == bipush.code) {
        u1 data = code[pc++];
        stack.push_back((int) data);
        
    }else if (opcode == invokestatic.code ||
              opcode == invokevirtual.code) {
        if (opcode == invokestatic.code)
            pc += invokestatic.argc;
        else if (opcode == invokevirtual.code)
            pc += invokevirtual.argc;
        int a;
        a = stack.back();
        stack.pop_back();
        if (verbose)
            cout << "println: ";
        cout << a << endl;
        
    }else if (opcode == return_.code) {
        pc = code.size();
    }else if (opcode == ireturn.code) {
        int a;
        a = stack.back();
        stack.pop_back();
        cout << "return: " << a << endl;
        
        pc = code.size();
        
    }else{
        cerr << "unknown opcode" << endl;
    }
}
