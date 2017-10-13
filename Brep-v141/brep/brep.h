#pragma once

struct BVertex;
struct BHalfEdge;
struct BEdge;
struct BLoop;
struct BFace;
struct BSolid;

struct BVertex
{
    BVertex(float x_, float y_, float z_) :x(x_), y(y_), z(z_), he(nullptr) {}

    float x;
    float y;
    float z;
    BHalfEdge *he;
};
