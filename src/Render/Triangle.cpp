#include "Triangle.h"


Triangle::Triangle() {
    valid = false;
    shared_material = nullptr;
}

float Triangle::intersect(Ray& r) {

    // vertices of the triangle
    Vector3 v0 = verts[0].Vector();
    Vector3 v1 = verts[1].Vector();
    Vector3 v2 = verts[2].Vector();

    // two edges of a triangle (NOTICE: CCW)
    Vector3 E1 = v2 - v1;
    Vector3 E2 = v0 - v2;

    // ray parameters
    const auto& o = r.origin;
    const auto& d = r.direction.unit();

    // compute normal of the triangle by edges
    Vector3 n = E1.cross(E2);

    // 1. find t on the planar
    // I comprehend it is a ray time broadcast time scalar 
    float t = (v0 - o).dot(n) / d.dot(n);

    // 2. check p is in the triangle or not
    // p is the point in this planar, but may not in the triangle,
    // so p should be checked
    Vector3 p = o + t * d;

    auto f1 = (v1 - v0).cross(p - v0).dot(n) < 0;
    auto f2 = (v2 - v1).cross(p - v1).dot(n) < 0;
    auto f3 = (v0 - v2).cross(p - v2).dot(n) < 0;

    // 3. check the direction and return t
    if (f1 == f2 && f2 == f3) return t;
    return (std::numeric_limits<float>::max)();
}

void Triangle::validate() {
    valid = true;
}

bool Triangle::isValid() {
    return valid;
}

Vector3 Triangle::Baricentric(Vector3& o) {

    // Input is the intersection between the ray and the triangle.

    // two edges of a triangle (NOTICE: CCW)
    Vector3 E1 = (verts[2] - verts[1]).Vector();
    Vector3 E2 = (verts[0] - verts[2]).Vector();

    // compute distances to edges
    Vector3 distance1 = o - verts[1].Vector();
    Vector3 distance2 = o - verts[2].Vector();

    // two normals
    Vector3 na = E1.cross(distance1);
    Vector3 nb = E2.cross(distance2);

    // compute normal of the triangle by edges
    Vector3 N = E1.cross(E2);

    // we can use na.length() / N.length() directly, 
    // but sqrt() is more costly 
    // while the normal is same plane as the na and nb, so using dot() and square() is more effective.
    // meanwhile, N.square() could also represent the area of the triangle. 
    // Thus, Area method and Length Method are same
    float normLenSquare = N.square();
    float alpha = N.dot(na) / normLenSquare;
    float beta = N.dot(nb) / normLenSquare;

    return Vector3{alpha, beta, 1 - alpha - beta};
}