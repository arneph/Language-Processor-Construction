#include "VCGGraph.h"

VCGGraph::VCGGraph() {
    nodes = vector<VCGNode *>();
    edges = vector<VCGEdge *>();
}

VCGGraph::~VCGGraph() {
    for (VCGNode *node : nodes) {
        delete node;
    }
    for (VCGEdge *edge : edges) {
        delete edge;
    }
}

const vector<VCGNode *>& VCGGraph::getNodes() {
    return nodes;
}

const vector<VCGEdge *>& VCGGraph::getEdges() {
    return edges;
}

void VCGGraph::addNode(VCGNode *node) {
    nodes.push_back(node);
}

void VCGGraph::addEdge(VCGEdge *edge) {
    edges.push_back(edge);
}

string VCGGraph::toVCGFormat() {
    stringstream ss;
    ss << "graph: { title: \"Graph\"\n";
    
    for (VCGNode *node : nodes) {
        ss << "node: {\n";
        ss << "title: \"" << (const void *)node << "\"\n";
        ss << "color: " + node->getColorText() << "\n";
        ss << "label: \n\"";
        ss << node->getText();
        ss << "\"\n";
        ss << "}\n";
    }
    
    for (VCGEdge *edge : edges) {
        ss << "edge: { sourcename: \"";
        ss << (const void *) edge->getSource();
        ss << "\" targetname: \"";
        ss << (const void *) edge->getTarget();
        ss << "\" arrowstyle: ";
        if (edge->isDirected())
            ss << "solid";
        else
            ss << "none";
        ss << "}\n";
    }
    
    ss << "}";
    
    return ss.str();
}
