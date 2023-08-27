//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2022
//
//  ------------------------
//  Vector4.cpp
//  ------------------------
//  
//  A minimal class for a 3D point in homogeneous coordinates
//  
///////////////////////////////////////////////////

#ifndef HOMOGENEOUS4_H
#define HOMOGENEOUS4_H

#include <iostream>
#include "Vector3.h"

// the class - we will rely on POD for sending to GPU
class Vector4
{ // Vector4
public:
    // the coordinates
    float x, y, z, w;

    // constructors
    Vector4();
    Vector4(float X, float Y, float Z, float W = 1.0);
    Vector4(const Vector3 &other);
    Vector4(const Vector4 &other);

    // routine to get a point by perspective division
    Vector3 Point() const;

    // routine to get a vector by dropping w (assumed to be 0)
    Vector3 Vector() const;

    Vector4 modulate(Vector4 b);

    Vector4 abs() const;

    // addition operator
    Vector4 operator +(const Vector4 &other) const;

    // addition operator
    Vector4 operator +=(const Vector4 &other);

    // subtraction operator
    Vector4 operator -(const Vector4 &other) const;

    // multiplication operator
    Vector4 operator *(float factor) const;

    // division operator
    Vector4 operator /(float factor) const;

    // operator that allows us to use array indexing instead of variable names
    float &operator [] (const int index);
    const float &operator [] (const int index) const;

}; // Vector4

// multiplication operator
Vector4 operator *(float factor, const Vector4 &right);

// stream input
std::istream & operator >> (std::istream &inStream, Vector4 &value);

// stream output
std::ostream & operator << (std::ostream &outStream, const Vector4 &value);

#endif
