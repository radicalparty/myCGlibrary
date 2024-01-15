#include "CircularList.cpp"
#define clist CircularList
using ll = long long;
#define dim 2
#define cqiter clist<vertex>::Iterator<vertex>
using namespace std;

static ll tarea(const ll* a, const ll* b, const ll* c){
    return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
}

//선분 교차 판정

static bool colinear(const ll* a, const ll* b, const ll* c){
    return tarea(a, b, c) == 0;
}

static bool between(const ll* a, const ll* b, const ll* c){
    if (!colinear(a, b, c)) return false;
    if (a[0] != b[0])   return min(a[0], b[0]) <= c[0] && c[0] <= max(a[0], b[0]);
    return min(a[1], b[1]) <= c[1] && c[1] <= max(a[1], b[1]);
}

static bool intersect(const ll* a, const ll* b, const ll* c, const ll* d){
    if (colinear(a, b, c) ||
        colinear(a, b, d) ||
        colinear(c, d, a) ||
        colinear(c, d, b)
            )   return between(a, b, c) || between(a, b, d) || between(c, d, a) || between(c, d, b);
    return (tarea(a, b, c) * tarea(a, b, d) < 0) && (tarea(c, d, a) * tarea(c, d, b) < 0);
}

//원형 큐를 이용한 polygon

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

    bool diagonalie(clist<vertex>::Iterator<vertex> a,clist<vertex>::Iterator<vertex> b){
        auto x = poly.begin();
        do{
            auto c = x.nxt();
            if (c != a || x != a || c != b || x != b){
                if (intersect(a->point, b->point, x->point, c->point))  return false;
            }
        }while (x != poly.begin());
        return true;
    }

    bool Incone(cqiter a, cqiter b){
        auto nxt = a.nxt();
        auto prv = a.prv();
        if (tarea(a->point, nxt->point, prv->point) >= 0){
            return (tarea(a->point, b->point, prv->point) > 0) && (tarea(b->point, a->point, nxt->point));
        }
        return !((tarea(a->point, b->point, nxt->point) >= 0) && tarea(b->point, a->point, prv->point) >= 0);
    }

    bool diagonal(cqiter a, cqiter b){
        return Incone(a, b) && Incone(b, a) && diagonalie(a, b);
    }
};







