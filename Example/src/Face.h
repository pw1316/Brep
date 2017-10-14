#ifndef _FACE_H_
#define _FACE_H_

#include <list>
#include "Loop.h"
class Mesh;

class Face {
public:
    Face(Mesh *m);
    ~Face();    
    void addLoop(Loop *l);
    
    int getId();

    int id;

    //Lista de loops
    std::list<Loop*> loops;
    //Loop exterior, que define a face
    Loop *outter;
    Mesh *solid;
};

#endif
