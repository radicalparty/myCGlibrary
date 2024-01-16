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


    //대각선-선분 교차 판정
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
    //내부/외부 여부 판정
    bool Incone(cqiter a, cqiter b){
        auto nxt = a.nxt();
        auto prv = a.prv();
        if (tarea(a->point, nxt->point, prv->point) >= 0){
            return (tarea(a->point, b->point, prv->point) > 0) && (tarea(b->point, a->point, nxt->point));
        }
        return !((tarea(a->point, b->point, nxt->point) >= 0) && tarea(b->point, a->point, prv->point) >= 0);
    }

    //대각선 판정
    bool diagonal(cqiter a, cqiter b){
        return Incone(a, b) && Incone(b, a) && diagonalie(a, b);
    }

    //Graham Scan 구현

    vector<vertex> graham_scan() const{
        vector<vertex> hull;
        auto p = poly.begin();
        vertex c = *p;
        do{
            if ((c.point[1] > p->point[1])
            || ((c.point[1] == p->point[1]) && (c.point[0] < p->point[0])))   c = *p;
            p = p.nxt();
        } while (p != poly.end());
        do{
            if (c != *p)    hull.emplace_back(*p);
            p = p.nxt();
        } while (p != poly.end());
        cout << "lowest point: " << c.point[0] << " " << c.point[1] << "\n";
        vector<vertex> st;
        sort(begin(hull), end(hull), [c](vertex& a, vertex& b){
           ll ax = c.point[0] - a.point[0], ay = c.point[1] - a.point[1];
           ll bx = c.point[0] - b.point[0], by = c.point[1] - b.point[1];
           if (ax * bx < 0) return ax < bx;
           else if (ax > 0){
               if (bx == 0) return false;
               else{
                   if (ay * bx == ax * by)  return ax < bx;
                   else return ay * bx < ax * by;
               }
           }
           else if (ax < 0){
               if (bx == 0) return true;
               else{
                   if (ay * bx == ax * by)  return ax > bx;
                   else return ay * bx > ax * by;
               }
           }
           else{
               if (bx == 0) return ay > by;
               else{
                   return bx > 0;
               }
           }
        });
        st.emplace_back(c);
        for (auto it = begin(hull); it != end(hull); it++){
            if (st.size() < 2){
                st.emplace_back(*it);
            }
            else{
                while (st.size() > 2 && tarea(st[st.size() - 2].point, st[st.size() - 1].point, it->point) <= 0){
                    st.pop_back();
                }
                st.emplace_back(*it);
            }
        }
        return st;
    }
};







