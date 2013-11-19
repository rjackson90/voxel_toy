#include <UnitTest++/UnitTest++.h>
#include <cstdio>

#include "fileops.h"

struct FileTestFixture
{
    FileTestFixture() : 
        binary_path("/tmp/Llyrdin-binarydata.tmp"), 
        text_path("/tmp/Llyrdin-textdata.tmp"),
        textData("The quick\t\n\t brown fox \n\njumped \t\t\tover the\t\n fence.\n\n")
    {
        std::fstream binfile(binary_path, std::fstream::binary | std::fstream::out);
        
        binfile.write((char*)&intData, sizeof(intData));
        binfile.write((char*)&longData, sizeof(longData));
        binfile.write((char*)&shortData, sizeof(shortData));
        binfile.write((char*)&charData, sizeof(charData));
        binfile.write((char*)&doubleData, sizeof(doubleData));
        binfile.write((char*)&floatData, sizeof(floatData));

        binfile.close();

        std::fstream textfile(text_path, std::fstream::out);
        textfile << textData;
        textfile.close();
    }

    ~FileTestFixture()
    {
        //remove(binary_path.c_str());
        //remove(text_path.c_str());
    }

    const std::string binary_path;
    const std::string text_path;
    static const size_t binary_len = sizeof(int) + sizeof(long) + sizeof(short) + 
        sizeof(char) + sizeof(double) + sizeof(float);
    
    const int intData = 12345;
    const long longData = 1234567890123490;
    const short shortData = -12;
    const char charData = 'A';
    const double doubleData = -12345.67891012345678910;
    const float floatData = 123.45;

    const std::string textData;
};

SUITE(FileOpsSuite)
{
    TEST_FIXTURE(FileTestFixture, getLength)
    {
        CHECK_EQUAL(static_cast<int>(binary_len), Core::getLength(binary_path));
    }

    TEST_FIXTURE(FileTestFixture, getBlob)
    {
        using namespace Core;

        char small_buffer[2];
        char full_buffer[binary_len];
        char large_buffer[binary_len+10];

        CHECK(!getBlob(binary_path, small_buffer, 2) );
        CHECK( getBlob(binary_path, full_buffer, binary_len) );
        CHECK( getBlob(binary_path, large_buffer, binary_len+10) );
    }

    TEST_FIXTURE(FileTestFixture, blobRead)
    {
        using namespace Core;

        char buffer[binary_len];
        char* b = &buffer[0];
        getBlob(binary_path, buffer, binary_len);

        int testInt;
        long testLong;
        short testShort;
        char testChar;
        double testDouble;
        float testFloat;

        blobRead(testInt, b);
        blobRead(testLong, b);
        blobRead(testShort, b);
        blobRead(testChar, b);
        blobRead(testDouble, b);
        blobRead(testFloat, b);

        CHECK_EQUAL(intData, testInt);
        CHECK_EQUAL(longData, testLong);
        CHECK_EQUAL(shortData, testShort);
        CHECK_EQUAL(charData, testChar);
        CHECK_EQUAL(doubleData, testDouble);
        CHECK_EQUAL(floatData, testFloat);
    }

    TEST_FIXTURE(FileTestFixture, getText)
    {
        using namespace Core;
        std::string testString = getText(text_path);

        CHECK_EQUAL(textData, testString);
    }
}
