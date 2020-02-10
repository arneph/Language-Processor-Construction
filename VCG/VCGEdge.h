#ifndef VCGEdge_h
#define VCGEdge_h

#include "VCGNode.h"

class VCGEdge {
    VCGNode *source;
    VCGNode *target;
    bool directed;
    
public:
    VCGEdge(VCGNode *s, VCGNode *t, bool directed=true);
    ~VCGEdge();
    
    VCGNode * getSource();
    VCGNode * getTarget();
    
    bool isDirected();
};

#endif /* VCGEdge_h */
