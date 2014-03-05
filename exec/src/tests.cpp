#include <unittest++/UnitTest++.h>
#include <boost/python.hpp>

int main()
{
    // Initialize Python interpreter ONCE
    Py_Initialize();

    return UnitTest::RunAllTests();
}
