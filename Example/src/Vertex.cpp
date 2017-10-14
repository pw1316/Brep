#include "Vertex.h"
#include "Mesh.h"
#include "HalfEdge.h"

Vertex::Vertex(float x, float y, float z, Mesh *m)
        : x(x), y(y), z(z), id(m->numVertices)
{
    m->addVertex(this);
    r = g = b = 0.0;
}

Vertex::Vertex(float x, float y, float z)
{
    this->x=x;
    this->y=y;
    this->z=z;
    r = g = b = 0.0;
}

int Vertex::getId() {
    return id;
}

void Vertex::move(float dx, float dy, float dz) {
    x += dx;
    y += dy;
    z += dz;
}
