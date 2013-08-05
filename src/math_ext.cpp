#include "math_ext.h"

/* Set all constants right off the bat */
const double Constants::PI = std::atan(1.0) * 4.0;

const Vector Constants::ORIGIN = Vector(0.0f, 0.0f, 0.0f);
const Vector Constants::UNIT_X = Vector(1.0f, 0.0f, 0.0f);
const Vector Constants::UNIT_Y = Vector(0.0f, 1.0f, 0.0f);
const Vector Constants::UNIT_Z = Vector(0.0f, 0.0f, 1.0f);

const float Constants::EPSILON = 0.0001f;

/*
 * Handy rad-to-deg and deg-to-rad conversions.
 * Functions GOOD Macros BAD
 */
double radians(double deg){ return deg * (Constants::PI/180.0); }
double degrees(double rad){ return rad * (180.0/Constants::PI); }

/*
 * Vector methods
 */
Vector::Vector(float X, float Y, float Z)
{
    x = X;
    y = Y;
    z = Z;
}

Vector::Vector()
{
    x = y = z = 0.0;
}

Vector::Vector(const glm::vec3 &vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

Vector Vector::operator+(const Vector &v) const
{
    Vector result;
    result.x = x + v.x;
    result.y = y + v.y;
    result.z = z + v.z;
    return result;
}

Vector Vector::operator*(float scalar) const
{
    Vector result;
    result.x = x * scalar;
    result.y = y * scalar;
    result.z = z * scalar;
    return result;
}

float Vector::dot(const Vector &v) const
{
    return x*v.x + y*v.y + z*v.z;
}

Vector Vector::cross(const Vector &v) const
{
    Vector result;
    result.x = y*v.z - v.y*z;
    result.y = z*v.x - v.z*x;
    result.z = x*v.y - v.x*y;
    return result;
}

float Vector::norm() const
{
    return sqrt(x*x + y*y + z*z);
}

Vector Vector::normalize() const
{
    Vector result(*this);
    float norm = this->norm();

    result.x = result.x / norm;
    result.y = result.y / norm;
    result.z = result.z / norm;

    return result;
}

glm::vec3 Vector::toGLMVec() const
{
    return glm::vec3(x, y, z);
}

/*
 * Quaternion methods
 */
Quaternion::Quaternion(float W, const Vector &v)
{
    w = W;
    x = v.x;
    y = v.y;
    z = v.z;
}

Quaternion::Quaternion()
{
    w = x = y = z = 0.0f;
}

Vector Quaternion::getV() const
{
    return Vector(this->x, this->y, this->z);
}

float Quaternion::norm2() const
{
    return w*w + x*x + y*y + z*z;
}

float Quaternion::norm() const
{
    return sqrt(this->norm2());
}

Quaternion Quaternion::operator*(float scalar) const
{
    Quaternion result;
    result.w = w * scalar;
    result.x = x * scalar;
    result.y = y * scalar;
    result.z = z * scalar;
    return result;
}

Quaternion Quaternion::operator*(const Quaternion &q) const
{
    Vector v1 = Vector(x, y, z);
    Vector v2 = Vector(q.x, q.y, q.z);

    return Quaternion(w*q.w - v1.dot(v2), 
                      v2*w + v1*q.w + v1.cross(v2));
}

Quaternion Quaternion::operator+(const Quaternion &q) const
{
    Quaternion result;
    result.w = w+q.w;
    result.x = x+q.x;
    result.y = y+q.y;
    result.z = z+q.z;
    return result;
}

Quaternion Quaternion::conjugate() const
{
    Quaternion result;
    result.w = w;
    result.x = -x;
    result.y = -y;
    result.x = -z;
    return result;
}

float Quaternion::dot(const Quaternion &q) const
{
    return w*q.w + x*q.x + y*q.y + z*q.z;
}

Vector Quaternion::rotate(const Vector &v) const
{
    Quaternion result = (*this) * Quaternion(0.0f, v) * this->conjugate();
    return Vector(result.x, result.y, result.z);
}

Quaternion Quaternion::slerp(const Quaternion &q, float t) const
{
    float flip = 1.0f;
    float cosine = this->dot(q);

    if (cosine < 0.0f)
    {
        cosine = -cosine;
        flip = -flip;
    }

    if ((1.0f - cosine) < Constants::EPSILON)
    {
        // If the quaternions are too close together, do an approximation
        return (*this) * (1.0f - t) + q * (t*flip);
    }

    float theta = (float) acos(cosine);
    float sine  = (float) sin(theta);
    float beta  = (float) sin((1-t)*theta) / sine;
    float alpha = (float) sin(t*theta) / sine * flip;

    return (*this) * beta + q * alpha;
}

void Quaternion::normalize()
{
    float mag = this->norm();
    
    this->w /= mag;
    this->x /= mag;
    this->y /= mag;
    this->z /= mag;
}

glm::mat4 Quaternion::toMatrix(const Vector &pos) const
{
    glm::mat4 result;
    
    result[0] = glm::vec4(1.0f - 2.0f*(y*y) - 2.0f*(z*z), 2.0f*x*y+2.0f*w*z, 2.0f*z*x-2.0f*w*y, 0.0f);
    result[1] = glm::vec4(2.0f*x*y-2.0f*w*z, 1.0f - 2.0f*(x*x) - 2.0f*(z*z), 2.0f*y*z+2.0f*w*x, 0.0f);
    result[2] = glm::vec4(2.0f*z*x+2.0f*w*y, 2.0f*y*z-2.0f*w*x, 1.0f - 2.0f*(x*x) - 2.0f*(y*y), 0.0f);
    result[3] = glm::vec4(pos.x, pos.y, pos.z, 1.0f);

    return result;
}
