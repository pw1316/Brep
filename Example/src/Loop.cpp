#include "Loop.h"
#include "Face.h"
#include "Mesh.h"

Loop::Loop(Face *f) {
    face = f;
    face->addLoop(this);

    this->id = f->solid->numLoops++;
    

    r = g = b= 0.6;
}

void Loop::setNormal() {
    HalfEdge *h = this->hed;
    HalfEdge *h2 = h->next;
    Vec3 v(h2->origin->x - h->origin->x,
           h2->origin->y - h->origin->y,
           h2->origin->z - h->origin->z);

    while((h = h->next) != this->hed) {
        h2 = h->next;
        Vec3 u(h2->origin->x - h->origin->x,
               h2->origin->y - h->origin->y,
               h2->origin->z - h->origin->z);
        //Verifica se dois vetores são colineares
        //v.u = |v||u|cos(theta)
        //Se os vetores forem colineares, cos(theta) == 1,
        //logo v.u == |v||u|
        if(dot(u, v) == u.length()*v.length())
            continue;
        else {
            this->normal = normalize(cross(v, u));
            break;
        }
    }
}

