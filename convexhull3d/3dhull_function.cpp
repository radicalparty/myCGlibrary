#include "3dhull_type.cpp"

//function
void ReadVertices(ll n);
void DoubleTriangle();
void ConstructHull();
void CleanUp();

void makeCcw(pface f, pedge e, pvertex p);
ll VolumeSign(pface f, pvertex p);
bool collinear(pvertex a, pvertex b, pvertex c);
bool AddOne(pvertex p);
pface makeFace(pvertex a, pvertex b, pvertex c, pface fold);
pface makeConeFace(pedge e, pvertex p);

void CleanEdges();
void CleanFaces();
void CleanVertices();

pvertex makeNullVertex();
pedge makeNullEdge();
pface makeNullFace();

//variable(원형 리스트)
pvertex vertices;
pedge edges;
pface faces;

//빈 vertex 생성
pvertex makeNullVertex(){
    pvertex c; NEW(c);
    c->duplicate = nullptr;
    c->onhull = !ONHULL;
    c->mark = !PROCESSED;
    ADD(vertices, c);
    return c;
}

//빈 edge 생성
pedge makeNullEdge(){
    pedge e; NEW(e);
    e->adjface[0] = e->adjface[1] = e->newface = nullptr;
    e->endpts[0] = e->endpts[1] = nullptr;
    e->del = !REMOVED;
    ADD(edges, e);
    return e;
}

//빈 face 생성
pface makeNullFace(){
    pface f; NEW(f);
    for (int i = 0; i < 3; i++){
        f->edge[i] = nullptr;
        f->vertex[i] = nullptr;
    }
    f->visible = !VISIBLE;
    ADD(faces, f);
    return f;
}

void convex3d(ll n){
    ReadVertices(n);
    DoubleTriangle();
    ConstructHull();
}

//입력
void ReadVertices(ll n){
    pvertex v;
    for (ll i = 0; i < n; i++){
        v = makeNullVertex();
        std::cin >> v->v[X] >> v->v[Y] >> v->v[Z]; v->vnum = i;
        ADD(vertices, v);
    }
}

//사면체 만들기
void DoubleTriangle(){
    pvertex v0, v1, v2, v3, v4;
    pface f0, f1 = nullptr;
    pedge e0, e1, e2, s;
    ll vol;

    v0 = vertices;
    while (collinear(v0, v0->next, v0->next->next)){//세 점이 일직선이 아닌, 즉 삼각형을 이루는 값을 찾기
        if (v0->next == vertices){//임의의 세 점에 대해서 collinear -> 볼록 껍질 없음.
            std::cout << "All points are collinear!\n";
            exit(0);
        }
        v0 = v0->next;
    }

    v1 = v0->next; v2 = v0->next->next;//삼각형을 이루는 세 점

    v0->mark = PROCESSED; v1->mark = PROCESSED; v2->mark = PROCESSED;

    f0 = makeFace(v0, v1, v2, f1);
    f1 = makeFace(v2, v1, v0, f0);

    //자신 쪽으로 맞닿는 자기 자신의 면 저장(역방향)
    f0->edge[0]->adjface[1] = f1;
    f0->edge[1]->adjface[1] = f1;
    f0->edge[2]->adjface[1] = f1;

    f1->edge[0]->adjface[1] = f0;
    f1->edge[1]->adjface[1] = f0;
    f1->edge[2]->adjface[1] = f0;

    v3 = v2->next;
    vol = VolumeSign(f0, v3);
    while (!vol){//점을 이동시키면서 사면체 존재 확인
        if (v3->next == v0){//사면체가 존재하지 않을 경우, 모든 점이 한 평면에 존재 -> 3d 볼록 껍질 없음
            std::cout << "All points are coplanar!\n";
            exit(0);
        }
        vol = VolumeSign(f0, v3);
        v3 = v3->next;
    }

    vertices = v3;
}

void ConstructHull(){//볼록 껍질 구성
    pvertex v, vnxt; int vol;
    v = vertices;

    do{
        vnxt = v->next;
        if (!v->mark){
            v->mark = PROCESSED;
            AddOne(v);
            CleanUp();
        }
        v = vnxt;
    } while (v != vertices);
}

ll VolumeSign(pface f, pvertex p){//행렬식 이용한 부피 계산 후 Coplanar 여부 확인
    double vol;
    double ax, ay, az, bx, by, bz, cx, cy, cz;

    ax = f->vertex[0]->v[X] - p->v[X];
    ay = f->vertex[0]->v[Y] - p->v[Y];
    az = f->vertex[0]->v[Z] - p->v[Z];

    bx = f->vertex[1]->v[X] - p->v[X];
    by = f->vertex[1]->v[Y] - p->v[Y];
    bz = f->vertex[1]->v[Z] - p->v[Z];

    cx = f->vertex[2]->v[X] - p->v[X];
    cy = f->vertex[2]->v[Y] - p->v[Y];
    cz = f->vertex[2]->v[Z] - p->v[Z];

    vol = ax * (by * cz - bz * cy) + ay * (bz * cx - bx * cz) + az * (bx * cy - by * cx);

    if (vol > 0.5)  return 1;
    else if (vol < -0.5) return -1;
    else    return 0;
}

//면 만들기
pface makeFace(pvertex a, pvertex b, pvertex c, pface fold){
    pface f;
    pedge e0, e1, e2;

    if (!fold){//만약 만들어지지 않은 면일 경우 면을 생성
        e0 = makeNullEdge(); e1 = makeNullEdge(); e2 = makeNullEdge();
    } else{//동일한 면을 복사 붙여넣기 하므로 edge는 똑값이 넣어줌
        e0 = fold->edge[2]; e1 = fold->edge[1]; e2 = fold->edge[0];
    }

    //edge 생성
    e0->endpts[0] = a; e0->endpts[1] = b;
    e1->endpts[0] = b; e1->endpts[1] = c;
    e2->endpts[0] = c; e2->endpts[1] = a;

    //면 생성
    f = makeNullFace();
    f->edge[0] = e0; f->edge[1] = e1; f->edge[2] = e2;
    f->vertex[0] = a; f->vertex[1] = b; f->vertex[2] = c;
    e0->adjface[0] = f; e1->adjface[0] = f; e2->adjface[0] = f;

    return f;
}

//세 점의 collinear 여부 확인
bool collinear(pvertex a, pvertex b, pvertex c){
    return ((c->v[Z] - a->v[Z]) * (b->v[Y] - a->v[Y]) == (c->v[Y] - a->v[Y]) * (b->v[Z] - a->v[Z]) &&
            (c->v[Z] - a->v[Z]) * (b->v[X] - a->v[X]) == (c->v[X] - a->v[X]) * (b->v[Z] - a->v[Z]) &&
            (c->v[Y] - a->v[Y]) * (b->v[X] - a->v[X]) == (c->v[X] - a->v[X]) * (b->v[Y] - a->v[Y])
    );
}

//Hull에 점을 넣는 과정
bool AddOne(pvertex p){
    pface f; pedge e, tmp; bool vis = false;

    f = faces;
    do{
        //모든 면에 대해서 점이 내부에 있는지, 외부에 있는지 확인
        if (VolumeSign(f, p) < 0){
            f->visible = VISIBLE;
            vis = true;
        }
        f = f->next;
    } while (f != faces);

    //만약 어디에서나 true, 즉 점이 내부에 존재할 경우, p를 제거해도 상관 없음
    if (!vis){
        p->onhull = !ONHULL; return false;
    }

    e = edges;
    do{
        tmp = e->next;
        //인접한 두 면이 모두 p에서 visible할 때 -> 이제 모서리 e는 볼록 껍질을 구성할 수 없음
        if (e->adjface[0]->visible && e->adjface[1]->visible){
            e->del = REMOVED;
        }
        //인접한 두 면 중 하나만 p에서 visible할 때 -> p와 e의 새로운 면을 만들자!
        else if (e->adjface[0]->visible || e->adjface[1]->visible){
            e->newface = makeConeFace(e, p);
        }
        e = tmp;
    } while (e != edges);

    return true;
}

pface makeConeFace(pedge e, pvertex p){
    pedge new_edge[2];
    pface new_face;

    //두 개의 새로운 edge 생성
    for (int i = 0; i < 2; i++){
        //만약 edge가 존재하면, 복사
        if (!(new_edge[i] = e->endpts[i]->duplicate)){
            //아니면, 새로운 edge를 생성
            new_edge[i] = makeNullEdge();
            new_edge[i]->endpts[0] = e->endpts[i];
            new_edge[i]->endpts[1] = p;
            e->endpts[i]->duplicate = new_edge[i];
        }
    }

    //새로운 면 생성
    new_face = makeNullFace();
    new_face->edge[0] = e;
    new_face->edge[1] = new_edge[0];
    new_face->edge[2] = new_edge[1];
    makeCcw(new_face, e, p);

    //new_face를 edge의 인접면에 저장
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            if (!new_edge[i]->adjface[j]){
                new_edge[i]->adjface[j] = new_face;
                break;
            }
        }
    }
    return new_face;
}

void makeCcw(pface f, pedge e, pvertex p){
    pface fv; int i = 0;
    //p에서 볼 수 있는 면 선택(없어지는 면)
    if (e->adjface[0]->visible) fv = e->adjface[0];
    else fv = e->adjface[1];

    for (i = 0; fv->vertex[i] != e->endpts[0]; i++) ;

    //시계 방향, 반시계 방향 구분해서 꼭짓점 대입
    if (fv->vertex[(i + 1) % 3] != e->endpts[1]){
        f->vertex[0] = e->endpts[1];
        f->vertex[1] = e->endpts[0];
    }
    else{
        f->vertex[0] = e->endpts[0];
        f->vertex[1] = e->endpts[1];
        swap(f->edge[1], f->edge[2]);
    }

    f->vertex[2] = p;
}

//새 볼록 껍질에서 사라져야할 점, 선, 면 제거
void CleanUp(){
    CleanEdges();
    CleanFaces();
    CleanVertices();
}

//face 제거
void CleanFaces(){
    pface f, t;
    while (faces && faces->visible){
        f = faces; DELETE(faces, f); //faces의 head를 제거함
    }
    f = faces->next;
    do{
        if (f->visible){//visible한 face를 모두 제거
            t = f; f = f->next; DELETE(faces, t);
        }
        else    f = f->next;
    } while (f != faces);
}

void CleanEdges(){
    pedge e, t;

    e = edges;
    do{
        if (e->newface){
            //newface를 e의 어느 인접면에 대입하는 지 결정함
            if (e->adjface[0]->visible) e->adjface[0] = e->newface;
            else    e->adjface[1] = e->newface;
            e->newface = nullptr;
        }
    } while (e != edges);

    while (edges && edges->del){//원형 리스트에서 지워야 하는 edge 제거
        e = edges; DELETE(edges, e);
    }

    e = edges->next;
    do{
        if (e->del){//지워야 하는 edge 제거
            t = e; e = e->next; DELETE(edges, t);
        }
        else e = e->next;
    } while (e != edges);
}

void CleanVertices(){
    pedge e; pvertex v, t;
    e = edges;
    do{
        e->endpts[0]->onhull = ONHULL; e->endpts[1]->onhull = ONHULL; e = e->next;
    } while (e != edges);

    while (vertices && vertices->mark && !vertices->onhull){
        v = vertices;
        DELETE(vertices, v);
    }
    v = vertices->next;
    do{
        if (v->mark && !v->onhull){
            t = v; v = v->next; DELETE(vertices, t);
        }
        else{
            v = v->next;
        }
    } while (v != vertices);

    //초기화
    v = vertices;
    do{
        v->duplicate = NULL;
        v->onhull = !ONHULL;
        v = v->next;
    } while (v != vertices);
}

