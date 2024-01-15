#include "polygon.cpp"
#define endl "\n"
using namespace std;
int main() {
    vertex a = {0, 0, 0, 0};
    vertex b = {1, 0, 1, 0};
    vertex c = {2, 1, 1, 0};
    polygon poly;
    poly.poly.insert(a);
    poly.poly.insert(b);
    poly.poly.insert(c);
    cout << poly.AreaPoly();
    return 0;
}
