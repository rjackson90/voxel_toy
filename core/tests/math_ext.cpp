#include <UnitTest++/UnitTest++.h>

#include "math_ext.h"

static const float tolerance = 0.00001f;

SUITE(MathExtensions)
{
    TEST(VectorAddition)
    {
        Vector result = Constants::UNIT_X + Constants::UNIT_Y;
        
        CHECK_CLOSE(result.x, 1.0f, tolerance);
        CHECK_CLOSE(result.y, 1.0f, tolerance);
        CHECK_CLOSE(result.z, 0.0f, tolerance);
    }

    TEST(VectorScaling)
    {
        Vector result = Constants::UNIT_X * 5.0f;

        CHECK_CLOSE(result.x, 5.0f, tolerance);
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
}
