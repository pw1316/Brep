#include "brep_struct.hpp"

BHalfEdge *BLoop::findHalfEdgeWithVertex(BVertex *startVertex)
{
    BHalfEdge *he = this->firstHalfEdge;
    do
    {
        if (he->vertex == startVertex)
            return he;
        he = he->next;
    } while (he != this->firstHalfEdge);
    return nullptr;
}
