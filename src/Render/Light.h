#ifndef LIGHT_H
#define LIGHT_H

#include "../Math/Vector4.h"

class Light
{

public:

enum LightType{
    Point,
    Area,
    Directional
};

private:
    Vector4 lightPosition;
    Vector4 lightDirection;
    Vector4 lightColor;
    Vector4 tangent1;
    Vector4 tangent2;
    LightType type;


public:
    Light(LightType type, Vector4 color, Vector4 pos, Vector4 dir, Vector4 tan1, Vector4 tan2);
    Vector4 GetPosition();
    Vector4 GetPositionCenter();

    bool enabled;

    inline Vector4 GetColor(){return lightColor;}

};

#endif // LIGHT_H
