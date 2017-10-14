#ifndef _EDGE_H_
#define _EDGE_H_

#include "HalfEdge.h"

class Mesh;

class Edge {
public:
    Edge(Mesh *m);
    ~Edge();    
    int id;
    HalfEdge *hed1, *hed2;
    float r, g, b;
};


#endif
