//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
////////////////////////////////////////////////////////////////////////


#include "Vector4.h"
#include "math.h"
#include <iomanip>

// constructors
Vector4::Vector4()
        :
        x(0.0),
        y(0.0),
        z(0.0),
        w(0.0) {}

Vector4::Vector4(float X, float Y, float Z, float W)
        :
        x(X),
        y(Y),
        z(Z),
        w(W) {}

Vector4::Vector4(const Vector3 &other)
        :
        x(other.x),
        y(other.y),
        z(other.z),
        w(1) {}

Vector4::Vector4(const Vector4 &other)
        :
        x(other.x),
        y(other.y),
        z(other.z),
        w(other.w) {}

// routine to get a point by perspective division
Vector3 Vector4::Point() const { // Vector4::Point()
    Vector3 returnVal(x / w, y / w, z / w);
    return returnVal;
} // Vector4::Point()

// routine to get a vector by dropping w (assumed to be 0)
Vector3 Vector4::Vector() const { // Vector4::Vector()
    Vector3 returnVal(x, y, z);
    return returnVal;
} // Vector4::Vector()


Vector4 Vector4::modulate(Vector4 b) { // Vector4::Vector()
    return Vector4(x * b.x, y * b.y, z * b.z, w * b.w);
} // Vector4::Vector()

// addition operator
Vector4 Vector4::operator+(const Vector4 &other) const { // Vector4::operator +()
    Vector4 returnVal(x + other.x, y + other.y, z + other.z, w + other.w);
    return returnVal;
} // Vector4::operator +()

// subtraction operator
Vector4 Vector4::operator-(const Vector4 &other) const { // Vector4::operator -()
    Vector4 returnVal(x - other.x, y - other.y, z - other.z, w - other.w);
    return returnVal;
} // Vector4::operator -()

// multiplication operator
Vector4 Vector4::operator*(float factor) const { // Vector4::operator *()
    Vector4 returnVal(x * factor, y * factor, z * factor, w * factor);
    return returnVal;
} // Vector4::operator *()

// division operator
Vector4 Vector4::operator/(float factor) const { // Vector4::operator /()
    Vector4 returnVal(x / factor, y / factor, z / factor, w / factor);
    return returnVal;
} // Vector4::operator /()

// operator that allows us to use array indexing instead of variable names
float &Vector4::operator[](const int index) { // operator []
    // use default to catch out of range indices
    // we could throw an exception, but will just return the 0th element instead
    switch (index) { // switch on index
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
            // actually the error case
        default:
            return x;
    } // switch on index
} // operator []

// operator that allows us to use array indexing instead of variable names
const float &Vector4::operator[](const int index) const { // operator []
    // use default to catch out of range indices
    // we could throw an exception, but will just return the 0th element instead
    switch (index) { // switch on index
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
            // actually the error case
        default:
            return x;
    } // switch on index
}

Vector4 Vector4::abs() const {
    return {std::fabs(x), std::fabs(y), std::fabs(z),std::fabs(w)};
}

Vector4 Vector4::operator+=(const Vector4 &other) {
    return {x += other.x, y += other.y, z += other.z, w += other.w};
}
// operator []

// multiplication operator
Vector4 operator*(float factor, const Vector4 &right) { // operator *
    // scalar multiplication is commutative, so flip & return
    return right * factor;
} // operator *

// stream input
std::istream &operator>>(std::istream &inStream, Vector4 &value) { // stream output
    inStream >> value.x >> value.y >> value.z >> value.w;
    return inStream;
} // stream output

// stream output
std::ostream &operator<<(std::ostream &outStream, const Vector4 &value) { // stream output
    outStream << std::setprecision(4) << value.x << " " << std::setprecision(4) << value.y << " "
              << std::setprecision(4) << value.z << " " << std::setprecision(4) << value.w;
    return outStream;
} // stream output
        
