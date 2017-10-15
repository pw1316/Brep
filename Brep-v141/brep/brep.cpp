#include "brep.hpp"

#include "brep_struct.hpp"

void Brep::mvfs(float x, float y, float z)
{
    BSolid *solid = new BSolid();
    BFace *face = new BFace();
    BLoop *loop = new BLoop();
    BHalfEdge *halfEdge = new BHalfEdge();
    BVertex *vertex = new BVertex(x, y, z);

    solids.push_back(solid);

    solid->faces.push_back(face);
    face->solid = solid;

    face->outLoop = loop;
    face->loops.push_back(loop);
    loop->face = face;

    brep_list_insert(brep_list_end(&loop->halfEdges), halfEdge);
    halfEdge->loop = loop;
    vertex->halfEdge = halfEdge;
    solid->vertices.push_back(vertex);

    halfEdge->vertex = vertex;
    halfEdge->edge = nullptr;
}
