#ifndef Instruction_h
#define Instruction_h

#include <iomanip>
#include <sstream>
#include <vector>

#include "Value.h"

using namespace std;

typedef enum Operator {
    Add,
    Subtract,
    Muliply,
    Divide,
    Negative,
    LeftShift,
    RightShift,
    Increment,
    Compare,
    Branch,
    BranchNotEqual,
    BranchEqual,
    BranchGreaterThan,
    BranchGreaterThanOrEqual,
    BranchLessThanOrEqual,
    BranchLessThan,
    ProcedureCall,
    Return,
    End,
    Move,
    Phi
} Operator;
static const char * OperatorStrings[] = {
    "add", "sub", "mul", "div", "neg", "lsl", "lsr", "inc",
    "cmp", "bra", "bne", "beq", "bgt", "bge", "ble", "blt",
    "cll", "ret", "end", "mov", "phi"
};

class Instruction {
    int number;
    
    Operator op;
    vector<Value *> arguments;
    Value *result;
    
public:
    Instruction(int num,
                Operator op,
                initializer_list<Value *> args);
    ~Instruction();
    
    string toString();
    bool computationEquals(Instruction *instr);
    
    int getNumber();
    Operator getOperator();
    vector<Value *>& getArguments();
    bool hasResult();
    Value * getResult();
    void setResult(Value *result);
};

#endif /* Instruction_h */
