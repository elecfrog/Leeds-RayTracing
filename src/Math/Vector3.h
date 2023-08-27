//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2022
//
//  ------------------------
//  Vector3.h
//  ------------------------
//  
//  A minimal class for a point in Cartesian space
//  
///////////////////////////////////////////////////

#ifndef CARTESIAN3_H
#define CARTESIAN3_H

#include <iostream>

class Vector4;

// the class - we will rely on POD for sending to GPU
class Vector3 { // Vector3
public:
    // the coordinates
    float x, y, z;

    // constructors
    Vector3();

    Vector3(float X, float Y, float Z);

    Vector3(const Vector3 &other);

    Vector3(const Vector4 &other);

    // equality operator
    bool operator==(const Vector3 &other) const;

    // addition operator
    Vector3 operator+(const Vector3 &other) const;

    // subtraction operator
    Vector3 operator-(const Vector3 &other) const;

    // multiplication operator
    Vector3 operator*(float factor) const;

    // multiplication operator
    Vector3 operator*(const Vector3 &right);

    // division operator
    Vector3 operator/(float factor) const;

    // dot product routine
    float dot(const Vector3 &other) const;
    
    
    // cross product routine
    Vector3 cross(const Vector3 &other) const;

    // routine to find the length
    float length() const;

    float square() const;

    inline Vector3 wiseAbs() const
    {
        return Vector3(fabs(x), fabs(y), fabs(z));
    }

    // normalisation routine
    Vector3 unit() const;

    // operator that allows us to use array indexing instead of variable names
    float &operator[](const int index);

    const float &operator[](const int index) const;

}; // Vector3

// multiplication operator
Vector3 operator*(float factor, const Vector3 &right);


// stream input
std::istream &operator>>(std::istream &inStream, Vector3 &value);

// stream output
std::ostream &operator<<(std::ostream &outStream, const Vector3 &value);

#endif
