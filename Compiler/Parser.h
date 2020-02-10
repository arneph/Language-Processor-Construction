#ifndef Parser_hpp
#define Parser_hpp

#include <map>
#include <set>
#include <vector>

#include "../Java/Java.h"
#include "../Intermediate Representation/IR.h"

using namespace std;

class Parser {
    JavaClass *javaClass;
    
    Program *ssaProgram;
    
    void parse();
    void determineDomTree();
    void determineDomFrontiers();
    
    // Dom Tree Calculation:
    // See "A Fast Algorithm for Finding Dominators in a Flowgraph" by Lengauer and Tarjan
    // http://delivery.acm.org/10.1145/360000/357071/p121-lengauer.pdf?ip=128.195.98.225&id=357071&acc=ACTIVE%20SERVICE&key=CA367851C7E3CE77%2EE385B6E260950907%2E4D4702B0C3E38B35%2E4D4702B0C3E38B35&__acm__=1558240503_ad04563576671a5c6b0a94da62cdc614
    vector<int> treeOrder;   // tree index   -> vertex index
    vector<int> treeParent;  // vertex index -> vertex index
    vector<int> sdom;        // vertex index -> tree index
    vector<int> idom;        // vertex index -> vertex index
    vector<set<int>> bucket; // vertex index -> vertex indices
    vector<int> ancestor;    // vertex index -> vertex index
    vector<int> label;       // vertex index -> vertex index
    
    void findDFSTree();
    void link(int v, int w);
    void compress(int v);
    int eval(int v);
    void findImplicitIDoms();
    void findExplicitIDoms();
    
public:
    Parser(JavaClass *c);
    ~Parser();

    Program * getProgram();
    
};

#endif /* Parser_h */
