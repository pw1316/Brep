#pragma once
#include "brep_struct.hpp"

class Brep
{
public:
    Brep() {}

    void mvfs(float x, float y, float z);
    BHalfEdge *mev(BLoop *loop, BVertex *vertex, float x, float y, float z);

    std::list<BSolid *> solids;
};