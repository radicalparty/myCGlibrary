#include "polygon.cpp"
#define endl "\n"
using namespace std;
int main() {
    vertex a = {0, 0, 0, 0};
    vertex b = {1, 0, 2, 0};
    vertex c = {2, 2, 2, 0};
    vertex d = {3, -2, 2, 0};
    vertex e = {4, 0, 4, 0};
    vertex f = {5, -2, 3, 0};
    polygon poly;
    poly.poly.insert(a);
    poly.poly.insert(b);
    poly.poly.insert(c);
    poly.poly.insert(d);
    poly.poly.insert(e);
    poly.poly.insert(f);
    auto x = poly.graham_scan();
    for (auto v: x){
        cout << v.point[0] << " " << v.point[1] << "\n";
    }
    return 0;
}
