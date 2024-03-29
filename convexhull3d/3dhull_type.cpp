#include <bits/stdc++.h>
using namespace std;

typedef struct vertex3_ tvertex;
typedef struct edge3_ tedge;
typedef struct face3_ tface;

typedef tvertex *pvertex;
typedef tedge *pedge;
typedef tface *pface;
using ll = long long;
#define X 0
#define Y 1
#define Z 2

#define ONHULL true
#define REMOVED true
#define VISIBLE true
#define PROCESSED true

template<typename T>
void swap(T* a, T* b){
    T tmp = *a; *a = *b; *b = tmp;
}

struct vertex3_{//정점
    ll v[3], vnum;
    bool onhull, mark;
    pedge duplicate;
    pvertex prev, next;
};

struct edge3_{//모서리
    pface adjface[2];
    pvertex endpts[2];
    pface newface;
    bool del;
    pedge prev, next;
};

struct face3_{//면
    pedge edge[3];
    pvertex vertex[3];
    pface prev, next;
    bool visible;
};

template<typename T>
void NEW(T*& p){//값을 할당
    p = new T();
}

template<typename T>
void DELETE(T*& head, T*& p){//값을 제거하고 원형 리스트 유지
    if (head == head->next) head = NULL;
    else if (p == head) head = head->next;
    p->next->prev = p->prev;
    p->prev->next = p->next;
    free(p);
}
template<typename T>
void ADD(T*& head, T*& p){//값을 원형 리스트에 추가
    if (head){
        p->next = head;
        p->prev = head->prev;
        (head->prev)->next = p;
        head->prev = p;
    }
    else{
        head = p;
        head->next = p;
        head->prev = p;
    }
}

void print_face(pface p){
    printf("Vertex of Face:\n");
    for (auto& i: p->vertex){
        cout << "Point: (" << i->v[0] << ", " << i->v[1] << ", " << i->v[2] << ")\n";
    }
    printf("--------------\n");
}