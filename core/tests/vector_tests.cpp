#include <unittest++/UnitTest++.h>

#include "math_ext.h"

static const float tolerance = 0.00001f;
static const std::string serial("1.0 2.0 3.0");

SUITE(VectorSuite)
{
    TEST(FromString)
    {
        Vector test = Vector::from_string(serial);
        CHECK_CLOSE(test.x, 1.0f, tolerance);
        CHECK_CLOSE(test.y, 2.0f, tolerance);
        CHECK_CLOSE(test.z, 3.0f, tolerance);
    }

    TEST(Construction)
    {
        Vector result;

        CHECK_CLOSE(result.x, 0.0f, tolerance);
        CHECK_CLOSE(result.y, 0.0f, tolerance);
        CHECK_CLOSE(result.z, 0.0f, tolerance);

        Vector other(1.0f, 0.0f, -1.0f);
        CHECK_CLOSE(other.x, 1.0f, tolerance);
        CHECK_CLOSE(other.y, 0.0f, tolerance);
        CHECK_CLOSE(other.z, -1.0f, tolerance);

        Vector third(glm::vec3(1.0f, 0.0f, -1.0f));
        CHECK_CLOSE(third.x, 1.0f, tolerance);
        CHECK_CLOSE(third.y, 0.0f, tolerance);
        CHECK_CLOSE(third.z, -1.0f, tolerance);
    }

    TEST(Addition)
    {
        Vector result = Constants::UNIT_X + Constants::UNIT_Y;
        
        CHECK_CLOSE(result.x, 1.0f, tolerance);
        CHECK_CLOSE(result.y, 1.0f, tolerance);
        CHECK_CLOSE(result.z, 0.0f, tolerance);
    }

    TEST(AddAssign)
    {
        Vector result = Constants::UNIT_X;
        result += Constants::UNIT_Y;

        CHECK_CLOSE(result.x, 1.0f, tolerance);
        CHECK_CLOSE(result.y, 1.0f, tolerance);
        CHECK_CLOSE(result.z, 0.0f, tolerance);
    }

    TEST(Scaling)
    {
        Vector result = Constants::UNIT_X * 5.0f;

        CHECK_CLOSE(result.x, 5.0f, tolerance);
        CHECK_CLOSE(result.y, 0.0f, tolerance);
        CHECK_CLOSE(result.z, 0.0f, tolerance);

        result = Constants::UNIT_X / 5.0f;
        CHECK_CLOSE(result.x, 0.2f, tolerance);
        CHECK_CLOSE(result.y, 0.0f, tolerance);
        CHECK_CLOSE(result.z, 0.0f, tolerance);
    }

    TEST(DotProduct)
    {
	    Vector a = Constants::UNIT_X; 
	    Vector b = Vector(-1.0f,0.0f,0.0f);
	    float dot = a.dot(b);
	    CHECK_CLOSE(dot, -1.0f, tolerance);
    }

    TEST(CrossProduct)
    {
        Vector result = Constants::UNIT_X.cross(Constants::UNIT_Y);

        CHECK_CLOSE(result.x, 0.0f, tolerance);
        CHECK_CLOSE(result.y, 0.0f, tolerance);
        CHECK_CLOSE(result.z, 1.0f, tolerance);
    }

    TEST(Norm)
    {
        Vector result(2.0f, -2.0f, 0.0f);
        
        CHECK_CLOSE(result.norm(), sqrt(8.0f), tolerance);
    }

    TEST(Normalize)
    {
        Vector result(1356545354365.3432423, -32454354234.23543, 5423.23432541);
        
        CHECK_CLOSE(result.normalize().norm(), 1.0f, tolerance);
    }

    TEST(ToGLMVec)
    {
        Vector ref(123.45f, 67.89f, -0.987654321f);
        glm::vec3 result = ref.toGLMVec();

        CHECK_CLOSE(result.x, ref.x, tolerance);
        CHECK_CLOSE(result.y, ref.y, tolerance);
        CHECK_CLOSE(result.z, ref.z, tolerance);
    }
}
