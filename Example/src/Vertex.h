#ifndef _VERTEX_H_
#define _VERTEX_H_


class HalfEdge;
class Mesh;

class Vertex {
public:
    Vertex(float x, float y, float z, Mesh *m);
    Vertex(float x, float y, float z);

    int getId();
    void move(float dx, float dy, float dz);

    void setPosition(double x, double y, double z) {this->x=x; this->y=y; this->z=z; }

    int id;
    float x, y, z;
    //Ponteiro para a half edge que sai desse vértice
    HalfEdge *hed;

    float r, g, b;
};

#endif
