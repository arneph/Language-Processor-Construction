#include "VCGEdge.h"

VCGEdge::VCGEdge(VCGNode *s, VCGNode *t, bool d) {
    source = s;
    target = t;
    directed = d;
}

VCGEdge::~VCGEdge() {}

VCGNode * VCGEdge::getSource() {
    return source;
}

VCGNode * VCGEdge::getTarget() {
    return target;
}

bool VCGEdge::isDirected() {
    return directed;
}
