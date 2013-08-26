#include "UnitTest++.h"

TEST(EpicFailure)
{
    CHECK(false);
}

TEST(EpicWin)
{
    CHECK(true);
}

int main()
{
    return UnitTest::RunAllTests();
}
