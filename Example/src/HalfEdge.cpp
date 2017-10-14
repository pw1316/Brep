#include "HalfEdge.h"
#include "Loop.h"
#include "Edge.h"
#include "Vertex.h"


HalfEdge::HalfEdge() {

}

HalfEdge* addhe(Edge *e, Vertex *v, HalfEdge *h, HEdSign sign) {
    HalfEdge *he;
    if (h->edge == 0)
        he = h;
    else {
        he = new HalfEdge;
        h->prev->next = he;
        he->prev = h->prev;
        h->prev = he;
        he->next = h;
    }

    he->edge = e;
    he->origin = v;
    he->loop = h->loop;
    if(sign == PLUS)
        e->hed1 = he;
    else 
        e->hed2 = he;

    return he;
}

HalfEdge::~HalfEdge() {
}

HalfEdge* HalfEdge::mate() {
    return (this == edge->hed1) ? this->edge->hed2 : this->edge->hed1;
}

HalfEdge* delhe(HalfEdge *he) {
    if(he->edge == 0) {
        delete he;
    } else if (he->next == he) {
        he->edge = 0;
        return he;
    }
    else {
        he->prev->next = he->next;
        he->next->prev = he->prev;
        delete he;
        return he->prev;
    }
}

