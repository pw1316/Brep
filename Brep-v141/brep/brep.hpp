#pragma once
#include "brep_struct.hpp"

class Brep
{
public:
    Brep() {}

    void mvfs(float x, float y, float z);
    BHalfEdge *mev(BLoop *loop, BVertex *vertex, float x, float y, float z);
    void mef(BLoop *outLoop, BVertex *vertex1, BVertex *vertex2);
    void mef(BLoop *outLoop, BVertex *vertex1, BVertex *vertex2, BVertex *vertex3, BVertex *vertex4);
    void kemr(BLoop *outLoop, BEdge *edge, BVertex *vertexOnOutLoop);
    void kfmhr(BFace *outFace, BFace *innerFace);

    void sweep(BSolid *solid, BFace *face, float x, float y, float z);

    void dump();
    std::list<BSolid *> solids;
};