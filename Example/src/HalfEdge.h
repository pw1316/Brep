#ifndef _HALFEDGE_H_
#define _HALFEDGE_H_

#include "Vertex.h"


class Edge;
class Loop;

enum HEdSign{ PLUS, MINUS };

class HalfEdge {
public:
    //HalfEdge(Edge *e, Vertex *v, HalfEdge *h, HEdSign sign);
    HalfEdge();
    ~HalfEdge();

    HalfEdge* mate();


    Vertex *origin;
    Loop *loop;
    Edge *edge;
    HalfEdge *next, *prev;
};

HalfEdge* addhe(Edge *e, Vertex *v, HalfEdge *h, HEdSign sign);
HalfEdge* delhe(HalfEdge *he);

#endif
