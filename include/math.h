#ifndef MATH_EXT_H
#define MATH_EXT_H

#include <math.h>

const float epsilon = 0.0001f;

struct Vector
{
    float x, y, z;

    Vector(float, float, float);
    Vector operator+(const Vector &);
    Vector operator*(float);
    float dot(const Vector &);
    Vector cross(const Vector &);
    float norm();
};

struct Quaternion
{
    float w, x, y, z;

    Quaternion(float, Vector);
    Vector getV();
    float norm();
    float norm2();
    Quaternion operator*(float);
    Quaternion operator*(const Quaternion &);
    Quaternion operator+(const Quaternion &);
    Quaternion conjugate(); // NOTE: for unit quaternions, conjugate == inverse
    float dot(const Quaternion &);
    Quaternion slerp(const Quaternion &, float);

    Vector rotate(const Vector &);
};

#endif /*MATH_EXT_H*/
