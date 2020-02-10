#include "Program.h"

Program::Program() {
    blocks = vector<BasicBlock *>();
}

Program::~Program() {}

string Program::toString() {
    stringstream ss;
    for (BasicBlock *block : blocks) {
        ss << block->toString() << endl;
    }
    return ss.str();
}

VCGGraph * Program::getCFG(bool detailed) {
    VCGGraph *graph = new VCGGraph();
    
    for (BasicBlock *block : blocks) {
        string text = (detailed) ?
            block->toString() : block->getTitle();
        VCGNode *blockNode = new VCGNode(text);
        graph->addNode(blockNode);
    }
    
    for (BasicBlock *source : blocks) {
        for (BasicBlock *target : source->getChildren()) {
            VCGNode *sourceNode =
                graph->getNodes()[source->getNumber()];
            VCGNode *targetNode =
                graph->getNodes()[target->getNumber()];
            VCGEdge *edge = new VCGEdge(sourceNode, targetNode);
            graph->addEdge(edge);
        }
    }
    
    return graph;
}

VCGGraph * Program::getDomTree() {
    VCGGraph *graph = new VCGGraph();
    
    for (BasicBlock *block : blocks) {
        string text = block->getTitle();
        VCGNode *blockNode = new VCGNode(text);
        graph->addNode(blockNode);
    }
    
    for (BasicBlock *source : blocks) {
        for (BasicBlock *target : source->getDominees()) {
            VCGNode *sourceNode =
            graph->getNodes()[source->getNumber()];
            VCGNode *targetNode =
            graph->getNodes()[target->getNumber()];
            VCGEdge *edge = new VCGEdge(sourceNode, targetNode);
            graph->addEdge(edge);
        }
    }
    
    return graph;
}

string Program::getDomFrontiersDescription() {
    stringstream ss;
    for (BasicBlock *block : blocks) {
        ss << setw(2) << block->getNumber() << ": ";
        int i = 0;
        for (BasicBlock *e : block->getDominanceFrontier()) {
            if (i++ > 0) ss << ", ";
            ss << setw(2) << e->getNumber();
        }
        ss << endl;
    }
    return ss.str();
}

const vector<BasicBlock *>& Program::getBlocks() {
    return blocks;
}

BasicBlock * Program::getStartBlock() {
    if (blocks.size() == 0) {
        return nullptr;
    }
    return blocks[0];
}

BasicBlock * Program::addBlock() {
    BasicBlock *newBlock = new BasicBlock((int) blocks.size());
    blocks.push_back(newBlock);
    return newBlock;
}

void Program::addCFGEdge(BasicBlock *parent,
                         BasicBlock *child) {
    parent->children.push_back(child);
    child->parents.push_back(parent);
}

void Program::removeCFGEdge(BasicBlock *parent, BasicBlock *child) {
    parent->children.erase(find(parent->children.begin(),
                                parent->children.end(),
                                child));
    child->parents.erase(find(child->parents.begin(),
                              child->parents.end(),
                              parent));
}

void Program::addDomTreeEdge(BasicBlock *dominator,
                             BasicBlock *dominee) {
    dominator->dominees.insert(dominee);
    dominee->dominator = dominator;
}

void Program::setDomFrontier(BasicBlock *dominator, set<BasicBlock *>& frontier) {
    dominator->dominanceFrontier = frontier;
}
