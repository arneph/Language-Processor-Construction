#ifndef VCGNode_h
#define VCGNode_h

#include <string>

using namespace std;

class VCGNode {
    string text;
    int color;
    
public:
    VCGNode(string txt, int color=-1);
    ~VCGNode();
    
    string getText();
    int getColor();
    string getColorText();
};

#endif /* VCGNode_h */
