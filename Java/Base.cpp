#include "Base.h"

u1 get_u1(ifstream& s) {
    u1 a = s.get();
    return (a << 0);
}

u2 get_u2(ifstream& s) {
    u1 a = s.get();
    u1 b = s.get();
    return (a << 8) | (b << 0);
}

u4 get_u4(ifstream& s) {
    u1 a = s.get();
    u1 b = s.get();
    u1 c = s.get();
    u1 d = s.get();
    return (a << 24) | (b << 16) | (c << 8) | (d << 0);
}
