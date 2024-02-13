#include <bits/stdc++.h>
#define REP(i, a, b) for (int i = a; i < b; i++)
#define X 0
#define Y 1
#define Z 2
#define DIM 3
using namespace std;
using ll = long long;
using ld = double;
typedef ll pt[DIM];
typedef ld pd[DIM];

double eps = 1e-12;
int V, F;
pt vertex[10001];
pt face[10001];

void NormalVec(const pt a, const pt b, const pt c, pd N);
ld Dot(const pt a, const pd b);
void subVec(const pt a, const pt b, pt c);
ll PlaneCoeff(const pt a, pd N, double* D);

void calculate_intersect(){
    //V = ReadVertices();
    //F = ReadFaces();
}

char SegPlaneInt(pt T, pt q, pt r, pd p, ll* m){
    pd N; ld D, num, denom, t; pt rq;

    *m = PlaneCoeff(T, N, &D);
    num = D - Dot(q, N); subVec(r, q, rq); denom = Dot(rq, N);

    if (fabs(denom) < eps){
        if (fabs(num) < eps)    return 'p';
        else    return '0';
    }
    else    t = num / denom;

    REP(i, 0, DIM)  p[i] = q[i] + t * (r[i] - q[i]);

    if ((0 < t) && (t < 1)) return '1';
    else if (fabs(num) < eps)   return 'q';
    else if (num == denom)  return 'r';
    return '0';
}

ll PlaneCoeff(const pt a, pd N, double* D){
    double t, biggest = 0; ll m = 0;
    NormalVec(vertex[a[0]], vertex[a[1]], vertex[a[2]], N);
    *D = Dot(vertex[a[0]], N);

    REP(i, 0, DIM){
        t = fabs(N[i]);
        if (t > biggest)    biggest = t; m = i;
    }
    return m;
}

void NormalVec(const pt a, const pt b, const pt c, pd N){
    N[X] = (c[Z] - a[Z]) * (b[Y] - a[Y]) - (b[Z] - a[Z]) * (c[Y] - a[Y]);
    N[Y] = (c[X] - a[X]) * (b[Z] - a[Z]) - (c[Z] - a[Z]) * (b[X] - a[X]);
    N[Z] = (c[Y] - a[Y]) * (b[X] - a[X]) - (c[X] - a[X]) * (b[Y] - a[Y]);
}

ld Dot(const pt a, const pd b){
    ld sum = 0;
    REP(i, 0, DIM)  sum += a[i] * b[i];
    return sum;
}

void subVec(const pt a, const pt b, pt c){
    REP(i, 0, DIM)  c[i] = a[i] - b[i];
}