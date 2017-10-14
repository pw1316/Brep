#include "Face.h"
#include "Mesh.h"

Face::Face(Mesh *m) : solid(m), id(m->numFaces)  {
    solid->addFace(this);
}

Face::~Face() {
    std::list<Loop*>::iterator iter;
    for(iter = loops.begin(); iter != loops.end(); iter++) {
        delete *iter;
    }
    loops.clear();
}

void Face::addLoop(Loop *l) {
    loops.push_back(l);
}

int Face::getId() {
    return id;
}
