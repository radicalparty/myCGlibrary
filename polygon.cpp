#include <bits/stdc++.h>
#include "CircularList.cpp"
#define clist CircularList
using namespace std;
struct vertex {
    int idx, x, y, z;
    bool ear;

    // ostream 연산자 오버로딩
    friend std::ostream& operator<<(std::ostream& os, const vertex& v) {
        os << "Vertex Information:" << endl;
        os << "Index: " << v.idx << endl;
        os << "(X, Y, Z) = (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
        os << "Ear: " << std::boolalpha << v.ear << endl;
        return os;
    }
};


