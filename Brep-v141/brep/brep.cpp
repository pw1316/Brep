#include "brep.hpp"

#include "brep_struct.hpp"

void Brep::mvfs(float x, float y, float z)
{
    BSolid *solid = new BSolid();
    BFace *face = new BFace();
    BLoop *loop = new BLoop();
    BHalfEdge *halfEdge = new BHalfEdge();
    BVertex *vertex = new BVertex(x, y, z);

    face->outLoop = loop;
    loop->halfEdge = halfEdge;
    halfEdge->loop = loop;
    halfEdge->vertex = vertex;
    halfEdge->edge = nullptr;
}
