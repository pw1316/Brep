#pragma once
#include "brep_struct.hpp"

class Brep
{
public:
    Brep() {}

    void mvfs(float x, float y, float z);
    BHalfEdge *mev(BLoop *loop, BVertex *vertex, float x, float y, float z);
    void mef(BLoop *outLoop, BVertex *vertex1, BVertex *vertex2);

    void dump();
    std::list<BSolid *> solids;
};