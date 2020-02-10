#ifndef VCGGraph_h
#define VCGGraph_h

#include <sstream>
#include <string>
#include <vector>

#include "VCGNode.h"
#include "VCGEdge.h"

using namespace std;

class VCGGraph {
    vector<VCGNode *> nodes;
    vector<VCGEdge *> edges;
    
public:
    VCGGraph();
    ~VCGGraph();
    
    const vector<VCGNode *>& getNodes();
    const vector<VCGEdge *>& getEdges();
    
    void addNode(VCGNode *node);
    void addEdge(VCGEdge *edge);
    
    string toVCGFormat();
};

#endif /* VCGGraph_h */
