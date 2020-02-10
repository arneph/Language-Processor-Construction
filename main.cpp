//
//  main.cpp
//  Green
//
//  Created by Arne Philipeit on 4/4/19.
//  Copyright © 2019 Arne Philipeit. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Java/Java.h"
#include "Interpreter/Interpreter.h"
#include "Compiler/Compiler.h"

using namespace std;

string baseDir =
    "/Users/arne/Documents/College Work/Year 4/COMPSCI 142B/";
string inDir = baseDir + "Input/tests_class/";
string outDir = baseDir + "Output/";

void printInt(int x) {
    printf("%d\n", x);
}

void writeASCIIFile(string path, string content) {
    ofstream outStream;
    outStream = ofstream(path);
    if (!outStream.is_open()) {
        cerr << "could not write to file: " << path << endl;
        return;
    }
    
    outStream << content;
}

void writeBinaryFile(string path, vector<uint8_t>content) {
    ofstream outStream;
    outStream = ofstream(path);
    if (!outStream.is_open()) {
        cerr << "could not write to file: " << path << endl;
        return;
    }
    
    for (uint8_t byte : content) {
        outStream << byte;
    }
}

JavaClass * load(string testName) {
    string classPath = inDir + testName + ".class";
    ifstream inStream(classPath);
    if (!inStream.is_open()) {
        cerr << "could not open class file: " << classPath << endl;
        return nullptr;
    }
    
    JavaClass *javaClass = new JavaClass(inStream);
    return javaClass;
}

void interpret(string testName, JavaClass *javaClass) {
    Interpreter interpreter(javaClass);
    interpreter.setVerbose(false);
    interpreter.run();
    
    while (!interpreter.isTerminated());
}

void writeProgramToFile(string path, Program *program) {
    string instr_txtPath = path + ".instr.txt";
    string instr_vcgPath = path + ".instr.vcg";
    string cfg_vcgPath = path + ".cfg.vcg";
    string dom_vcgPath = path + ".dom.vcg";
    string dfs_txtPath = path + ".dfs.txt";
    
    writeASCIIFile(instr_txtPath, program->toString());
    writeASCIIFile(instr_vcgPath, program->getCFG(true)->toVCGFormat());
    writeASCIIFile(cfg_vcgPath, program->getCFG(false)->toVCGFormat());
    writeASCIIFile(dom_vcgPath, program->getDomTree()->toVCGFormat());
    writeASCIIFile(dfs_txtPath, program->getDomFrontiersDescription());
}

void compile(string testName, JavaClass *javaClass) {
    string testDir = outDir + testName + "/";
    
    mkdir(testDir.c_str(), 07777);
    
    Parser parser(javaClass);
    Program *program = parser.getProgram();
    
    string inPath = testDir + "in";
    writeProgramToFile(inPath, program);
    
    SSAGenerator generator(program);
    
    string ssaGenPath = testDir + "ssa.gen";
    writeProgramToFile(ssaGenPath, program);
    
    SSAOptimizer optimizer(program);
    LiveRangeTable *liveRangeTable = optimizer.getLiveRangeTable();
    
    string ssaOptPath = testDir + "ssa.opt";
    writeProgramToFile(ssaOptPath, program);
    
    string trace1 = optimizer.getMoveInstructionElinimationTrace();
    string trace2 = optimizer.getConstantPropagationTrace();
    string trace3 = optimizer.getCopyPropagationTrace();
    string trace4 = optimizer.getUnusedInstructionEliminationTrace();
    
    string trace1Path = testDir + "trace.move_elim.txt";
    string trace2Path = testDir + "trace.const_prop.txt";
    string trace3Path = testDir + "trace.copy_prop.txt";
    string trace4Path = testDir + "trace.unused_elim.txt";
    
    writeASCIIFile(trace1Path, trace1);
    writeASCIIFile(trace2Path, trace2);
    writeASCIIFile(trace3Path, trace3);
    writeASCIIFile(trace4Path, trace4);
    
    string liveRangeTableText = liveRangeTable->toString();
    string liveRangeTablePath = testDir + "live_ranges.txt";
    writeASCIIFile(liveRangeTablePath, liveRangeTableText);
    
    MemoryAllocator memoryAllocator(program, liveRangeTable);
    
    string memPath = testDir + "mem";
    writeProgramToFile(memPath, program);
    
    string interferenceGraph = memoryAllocator.getInterferenceGraph()->toVCGFormat();
    string interferenceGraphVCG = testDir + "inteferences.vcg";
    writeASCIIFile(interferenceGraphVCG, interferenceGraph);
    
    char *JITCode = (char *)mmap(NULL, 1<<12, PROT_EXEC | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    
    MachineCodeGenerator machineCodeGenerator(program);
    
    machineCodeGenerator.patchPrintInt(JITCode, (void*) &printInt);
    
    string asm_bin = testDir + "asm.bin";
    string asm_txt = testDir + "asm.txt";
    
    writeBinaryFile(asm_bin, machineCodeGenerator.getFinalCode());
    writeASCIIFile(asm_txt, machineCodeGenerator.getBlockText());
    
    vector<uint8_t> code = machineCodeGenerator.getFinalCode();
    for (int i = 0; i < code.size(); i++) {
        JITCode[i] = code[i];
    }
    ((void(*)(void)) JITCode)();
}

int main(int argc, const char * argv[]) {
    int mode = 0;
    if (argc > 1 && strcmp(argv[1], "-c") == 0) {
        mode = 0;
    }else if (argc > 1 && strcmp(argv[1], "-i") == 0) {
        mode = 1;
    }
    
    for (int i = 0; i <= 7; i++) {
        string testName = "Test" + to_string(i);
        cout << testName << endl;
        
        JavaClass *javaClass = load(testName);
        
        if (mode == 0) {
            compile(testName, javaClass);
        }else if (mode == 1) {
            interpret(testName, javaClass);
        }
    }
    return 0;
}
