#ifndef _MESH_H_
#define _MESH_H_

#include <list>
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"
#include "BoundingBox.h"
#include "math/Quat.h"


class Mesh {
public:
    Mesh(int id);
    ~Mesh();
    int getId();

    void addVertex(Vertex *v);
    void addFace(Face *f);
    void addEdge(Edge *e);
    
    //Op. Geomtétricas
    void translate(float dx, float dy, float dz);
    void scale(float dx, float dy, float dz);
    void rotate(float angle, float x, float y, float z);


    int numFaces;
    int numVertices;
    int numLoops;
    int numEdges;
//TODO: encapsulamento
    std::list<Vertex*> vertices;
    std::list<Face*> faces;
    std::list<Edge*> edges;

    int id;
    BoundingBox bb;

    bool manifold;
    bool initialized;

    float r, g, b;
    //Informações sobre as operações geométricas
    //Translação
    float tx, ty, tz;
    //Escala
    float sx, sy, sz;
    //Rotação
    Quat orientation;
};

#endif
