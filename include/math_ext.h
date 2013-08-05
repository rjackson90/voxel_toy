#ifndef MATH_EXT_H
#define MATH_EXT_H

#include <math.h>
#include <glm/glm.hpp>

double radians(double);
double degrees(double);

/* Custom Vector object, because sometimes GLM sucks */
struct Vector
{
    float x, y, z;
    
    Vector();
    Vector(float, float, float);
    Vector(const glm::vec3 &);
    Vector operator+(const Vector &) const;
    Vector operator*(float) const;
    float dot(const Vector &) const;
    Vector cross(const Vector &) const;
    float norm() const;
    Vector normalize() const;
    glm::vec3 toGLMVec() const;

    // Useful constants
    static const Vector UNIT_X;
    static const Vector UNIT_Y;
    static const Vector UNIT_Z;
};

/* Representation of Quaternions, because GLM didn't meet requirements */
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
    glm::mat4 toMatrix(const Vector&) const;
};

/* Useful math constants */
struct Constants
{
    static const float EPSILON;

    static const Vector ORIGIN;
    static const Vector UNIT_X;
    static const Vector UNIT_Y;
    static const Vector UNIT_Z;

    static const double PI;
};

#endif /*MATH_EXT_H*/
