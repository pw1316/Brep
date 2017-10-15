#pragma once
#include "brep_struct.hpp"

class Brep
{
public:
    Brep() {}

    void mvfs(float x, float y, float z);

private:
    std::list<BSolid *> solids;
};