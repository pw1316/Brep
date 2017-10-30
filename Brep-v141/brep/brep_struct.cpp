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

BHalfEdge *BLoop::findHalfEdgeWithVertex(BVertex * startVertex, BVertex * endVertex)
{
    BHalfEdge *he = this->firstHalfEdge;
    do
    {
        if (he->vertex == startVertex && he->next->vertex == endVertex)
            return he;
        he = he->next;
    } while (he != this->firstHalfEdge);
    return nullptr;
}

BLoop *BFace::GetLoop(int index)
{
    std::list<BLoop *>::iterator it;
    int i = 0;
    for (it = loops.begin(), i = 0; it != loops.end(); ++it, ++i)
    {
        if (i == index)
        {
            return *it;
        }
    }
    return nullptr;
}

BFace *BSolid::GetFace(int index)
{
    std::list<BFace *>::iterator it;
    int i = 0;
    for (it = faces.begin(), i = 0; it != faces.end(); ++it, ++i)
    {
        if (i == index)
        {
            return *it;
        }
    }
    return nullptr;
}

BEdge *BSolid::GetEdge(int index)
{
    std::list<BEdge *>::iterator it;
    int i = 0;
    for (it = edges.begin(), i = 0; it != edges.end(); ++it, ++i)
    {
        if (i == index)
        {
            return *it;
        }
    }
    return nullptr;
}

BVertex *BSolid::GetVertex(int index)
{
    std::list<BVertex *>::iterator it;
    int i = 0;
    for (it = vertices.begin(), i = 0; it != vertices.end(); ++it, ++i)
    {
        if (i == index)
        {
            return *it;
        }
    }
    return nullptr;
}

BHalfEdge *BHalfEdge::bro()
{
    if (this->edge)
    {
        /* I'm A, return B; I'm B, return A */
        if (this->edge->halfEdgeA == this)
        {
            return this->edge->halfEdgeB;
        }
        else
        {
            return this->edge->halfEdgeA;
        }
    }
    /* First MSFV has virtual HE */
    return nullptr;
}
