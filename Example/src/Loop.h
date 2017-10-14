#ifndef _LOOP_H_
#define _LOOP_H_

#include "HalfEdge.h"
#include "math/Vec3.h"

class Face;

class Loop {
public:
    Loop(Face *f);
    
    void setNormal();

    Vec3 normal;
    
    //ID para o picking
    int id;
    //Cor
    float r, g, b;
    //void setFace(Face *f);
    HalfEdge *hed;
    Face *face;
};

#endif
