#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../Math/Vector4.h"
#include "../Math/Ray.h"
#include "Material.h"
#include "RGBAImage.h"

class Triangle
{
private:
    bool valid;

public:
    Vector4 verts[3];
    Vector4 normals[3];
    Vector4 colors[3];
    Vector3 uvs[3];

    Material *shared_material;

    Triangle();
    void validate();
    bool isValid();
    float intersect(Ray& r);

    Vector3 Baricentric(Vector3& o);

};

#endif // TRIANGLE_H
