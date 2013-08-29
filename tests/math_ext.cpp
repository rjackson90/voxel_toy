#include <UnitTest++/UnitTest++.h>

#include "math_ext.h"

static const float tolerance = 0.001f;

SUITE(MathExtensions)
{
    TEST(VectorAddition)
    {
        Vector result = Constants::UNIT_X + Constants::UNIT_Y;
        
        CHECK_CLOSE(result.x, 1.0f, tolerance);
        CHECK_CLOSE(result.y, 1.0f, tolerance);
        CHECK_CLOSE(result.z, 0.0f, tolerance);
    }
}
