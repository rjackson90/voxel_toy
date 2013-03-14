#ifndef MATH_EXT_H
#define MATH_EXT_H

#include <math.h>

const float epsilon = 0.0001f;

struct Vector
{
    float x, y, z;
    
    Vector();
    Vector(float, float, float);
    Vector operator+(const Vector &) const;
    Vector operator*(float) const;
    float dot(const Vector &) const;
    Vector cross(const Vector &) const;
    float norm() const;
};

struct Quaternion
{
    float w, x, y, z;
    
    Quaternion();
    Quaternion(float, const Vector &);
    Vector getV() const;
    float norm() const;
    float norm2() const;
    Quaternion operator*(float) const;
    Quaternion operator*(const Quaternion &) const;
    Quaternion operator+(const Quaternion &) const;
    Quaternion conjugate() const; // NOTE: for unit quaternions, conjugate == inverse
    float dot(const Quaternion &) const;
    Quaternion slerp(const Quaternion &, float) const;
    void normalize();
    Vector rotate(const Vector &) const;
};

#endif /*MATH_EXT_H*/
