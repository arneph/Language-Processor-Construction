#ifndef Interpreter_h
#define Interpreter_h

#include "../Java/Java.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;

class Interpreter {
    JavaClass *j_class;
    JavaMethod *j_main;
    
    bool running;
    bool terminating;
    bool terminated;
    thread interpret_thread;
    
    bool verbose;
    
    vector<u1> code;
    u2 pc;
    vector<u4> stack;
    vector<u4> locals;
    
    void interpret();
    
    void printInstruction();
    void executeInstruction();
    
public:
    Interpreter(JavaClass *c);
    ~Interpreter();
    
    bool isRunning();
    bool isTerminated();
    
    void run();
    void pause();
    void terminate();
    
    bool isVerbose();
    void setVerbose(bool v);
};

#endif /* Interpreter_h */
