#include "Edge.h"
#include "Mesh.h"

Edge::Edge(Mesh *m) : id(m->numEdges) 
{
    m->addEdge(this);
    r = g = b = 0.0;
}

Edge::~Edge() {
    delete hed1;
    delete hed2;
}
