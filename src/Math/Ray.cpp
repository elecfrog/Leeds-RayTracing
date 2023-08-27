#include "Ray.h"

Ray::Ray(Vector3 og, Vector3 dir, Type rayType)
{
    origin = og;
    direction = dir;
    ray_type = rayType;
}
