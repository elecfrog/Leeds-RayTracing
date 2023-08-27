//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
////////////////////////////////////////////////////////////////////////

#include "Light.h"
#include <math.h>

Light::Light(LightType type, Vector4 color, Vector4 pos, Vector4 dir, Vector4 tan1, Vector4 tan2)
{
    this->type = type;
    this->lightPosition = pos;
    this->lightDirection = dir;
    this->tangent1 = tan1;
    this->tangent2 = tan2;
    this->lightColor = color;
    enabled = false;
}
Vector4 Light::GetPositionCenter()
{
    return lightPosition;
}

Vector4 Light::GetPosition()
{
    if(type == LightType::Directional){
        return lightDirection;
    }
    //if its an area light lets do a random sampling for the position
    else if(type== LightType::Area)
    {
        float u = (-0.5f+(rand()/ float(RAND_MAX)));
        float v = (-0.5f+(rand()/ float(RAND_MAX)));
        Vector4 pos(lightPosition);
        pos = pos + u*tangent1;
        pos = pos + v*tangent2;
        return pos;
    }
    else {
        //for point light, we can also do a bit of montecarlo to vary the position given a "size"
        //and a random between 0 and size.
        float pi = float(2 * acos(0.0));
        //So we generate two angles for polar coordinates
        float theta = (pi*2.0f)*(rand()/ float(RAND_MAX));
        float phi = (pi*2.0f)*(rand()/ float(RAND_MAX));
        float r = 0.01f*(rand()/ float(RAND_MAX));


        //back to cartesian.
        float x = r*cos(phi)*sin(theta);
        float y = r*sin(phi)*sin(theta);
        float z = r*cos(theta);

        Vector3 out = lightPosition.Point() + Vector3(x, y, z);

        return out;
    }
}
