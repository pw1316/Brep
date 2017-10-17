#include "brep.hpp"

#include <assert.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

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

    loop->firstHalfEdge = halfEdge;
    halfEdge->loop = loop;
    halfEdge->prev = halfEdge;
    halfEdge->next = halfEdge;

    halfEdge->vertex = vertex;
    halfEdge->edge = nullptr;
    vertex->halfEdge = halfEdge;
    solid->vertices.push_back(vertex);
}

BHalfEdge *Brep::mev(BLoop *loop, BVertex *vertex, float x, float y, float z)
{
    BEdge *e = new BEdge();
    BHalfEdge *he = nullptr;
    BHalfEdge *he1 = nullptr;
    BHalfEdge *he2 = new BHalfEdge();
    BVertex *v = new BVertex(x, y, z);

    /* he1 may exists already */
    he = loop->firstHalfEdge;
    do
    {
        if (he->vertex == vertex)
            break;
        he = he->next;
    } while (he != loop->firstHalfEdge);
    assert(he->vertex == vertex);

    loop->face->solid->vertices.push_back(v);
    /* Virtual Half Edge(The first Half Edge of the Edge already exists) */
    if (he->edge == nullptr)
    {
        he1 = he;
        he1->next = he2;
        he1->prev = he2;
        he2->next = he1;
        he2->prev = he1;

        he2->loop = loop;
        he2->vertex = v;
        he1->edge = e;
        he2->edge = e;
    }
    /* Real Half Edge(Need to create new Half Edge) */
    else
    {
        he1 = new BHalfEdge();
        he1->next = he2;
        he1->prev = he->prev;
        he2->next = he;
        he2->prev = he1;
        he->prev->next = he1;
        he->prev = he2;

        he1->loop = loop;
        he2->loop = loop;
        he1->vertex = vertex;
        he2->vertex = v;
        he1->edge = e;
        he2->edge = e;
    }
    e->halfEdgeA = he1;
    e->halfEdgeB = he2;

    return he1;
}

//int main()
//{
//    Brep brep;
//    brep.mvfs(0, 0, 0);
//    brep.mev(brep.solids.front()->faces.front()->outLoop, brep.solids.front()->vertices.back(), 1, 0, 0);
//    brep.mev(brep.solids.front()->faces.front()->outLoop, brep.solids.front()->vertices.back(), 1, -1, 0);
//    brep.mev(brep.solids.front()->faces.front()->outLoop, brep.solids.front()->vertices.back(), 0, -1, 0);
//}
