#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

class Ray
{


public:
    enum Type{primary,secondary};
    Ray(Vector3 og, Vector3 dir, Type rayType);
    Vector3 origin;
    Vector3 direction;
    Type ray_type;

};

#endif // RAY_H
