#pragma once
#include <list>

#include "brep_list.hpp"

struct BVertex;
struct BHalfEdge;
struct BEdge;
struct BLoop;
struct BFace;
struct BSolid;

struct BVertex
{
    BVertex(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}

    float x;
    float y;
    float z;
    BHalfEdge *halfEdge = nullptr;
};

struct BHalfEdge
{
    BHalfEdge(){}

    BrepListNode node;
    BVertex *vertex = nullptr;
    BEdge *edge = nullptr;
    BLoop *loop = nullptr;
};

struct BEdge
{
    BEdge() {}

    BHalfEdge *halfEdgeA = nullptr;
    BHalfEdge *halfEdgeB = nullptr;
};

struct BLoop
{
    BLoop() {}

    BHalfEdge *halfEdge = nullptr;
    BFace *face;
};

struct BFace
{
    BFace() {}

    std::list<BLoop *> loops;
    BLoop *outLoop = nullptr;
    BSolid *solid;
};

struct BSolid
{
    BSolid() {}

    std::list<BFace *> faces;
    std::list<BEdge *> edges;
    std::list<BVertex *> vertices;
};
