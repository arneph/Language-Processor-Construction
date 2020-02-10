#ifndef Base_h
#define Base_h

#include <iostream>
#include <fstream>

using namespace std;

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

u1 get_u1(ifstream& s);
u2 get_u2(ifstream& s);
u4 get_u4(ifstream& s);

#endif /* Base_h */
