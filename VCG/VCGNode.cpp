#include "VCGNode.h"

VCGNode::VCGNode(string txt, int c) {
    text = txt;
    color = c;
}

VCGNode::~VCGNode() {}

string VCGNode::getText() {
    return text;
}

int VCGNode::getColor() {
    return color;
}

string VCGNode::getColorText() {
    switch (color) {
        case 0:
            return "yellow";
        case 1:
            return "green";
        case 2:
            return "turquoise";
        case 3:
            return "red";
        case 4:
            return "blue";
        case 5:
            return "magenta";
        default:
            return "white";
    }
}
