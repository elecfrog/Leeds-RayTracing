//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2020
//
//  ------------------------
//  Quaternion.h
//  ------------------------
//  
//  A class representing a quaternion
//  
///////////////////////////////////////////////////

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <stdio.h>
#include "Vector3.h"
#include "Vector4.h"

// forward declaration
class Quaternion;

#include "Matrix4.h"

class Quaternion
    { // class Quaternion
    public:
    // first three coordinates are imaginary parts 
    // last coordinate is real part
    Vector4 coords;

    // constructor: sets the quaternion to (0, 0, 0, 1)
    Quaternion();

    // constructor: sets the quaternion to (x, y, z, w)
    Quaternion(float x, float y, float z, float w);

    // Set to a pure scalar value
    Quaternion(float scalar);

    // Set to a pure vector value
    Quaternion(const Vector3 &vector);
    
    // Set to a homogeneous point
    Quaternion(const Vector4 &point);
    
    // Set to a rotation defined by a rotation matrix
    // WARNING: MATRIX MUST BE A VALID ROTATION MATRIX
    Quaternion(const Matrix4 &matrix);

    // Set to a rotation defined by an axis and angle
    Quaternion(const Vector3 &axis, float theta);

    // Copy another Quaternion & return self
    Quaternion &operator = (const Quaternion &other);
    
    // Computes the norm (sum of squares)
    float Norm() const;
    
    // Reduce to unit quaternion
    Quaternion Unit() const;
    
    // Conjugate the quaternion
    Quaternion Conjugate() const;
    
    // Invert the quaternion
    Quaternion Inverse() const;

    // Scalar right-multiplication
    Quaternion operator *(float scalar) const;

    // Scalar right-division
    Quaternion operator /(float scalar) const;

    // Adds two quaternions together
    Quaternion operator +(const Quaternion &other) const;

    // Subtracts one quaternion from another
    Quaternion operator -(const Quaternion &other) const;

    // Multiplies two quaternions together
    Quaternion operator *(const Quaternion &other) const;

    // Acts on a vector
    Vector3 Act(const Vector3 &vector) const;
    
    // Acts on a homogeneous point
    Vector4 Act(const Vector4 &point) const;
    
    // Returns the angle 2*theta of the action in degrees
    float AngleOfAction() const;
    
    // Returns the axis of rotation
    Vector3 AxisOfRotation() const;
    
    // Converts a quaternion to a rotation matrix
    Matrix4 GetMatrix() const;
    
    }; // class Quaternion

// Scalar left-multiplication
Quaternion operator *(float scalar, const Quaternion &quat);

// stream input
std::istream & operator >> (std::istream &inStream, Quaternion &quat);

// stream output
std::ostream & operator << (std::ostream &outStream, const Quaternion &quat);



#endif
