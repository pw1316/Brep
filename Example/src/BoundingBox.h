#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include "math/Vec3.h"

class BoundingBox {
public:
    BoundingBox() { };
    BoundingBox(const Vec3& pMin, const Vec3& pMax) {
        this->pMin = pMin;
        this->pMax = pMax;
    }

    Vec3 pMin, pMax;
};

#endif

