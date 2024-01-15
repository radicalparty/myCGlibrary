#include "CircularList.cpp"
#define clist CircularList
using ll = long long;
#define dim 2
using namespace std;

static ll tarea(const ll* a, const ll* b, const ll* c){
    return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
}

struct polygon{
    clist<vertex> poly;
    polygon(): poly() {}
    ll AreaPoly() const{
        ll sum = 0;
        auto p = poly.begin();
        auto a = p.nxt();
        do{
            sum += tarea(p->point, a->point, a.nxt()->point);
            a = a.nxt();
        } while (a.nxt() != p);
        return sum;
    }
};







