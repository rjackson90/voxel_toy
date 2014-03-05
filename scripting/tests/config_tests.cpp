#include <unittest++/UnitTest++.h>
#include <fstream>
#include <cstdio>

#include "config.h"

struct ConfigTestFixture
{
    ConfigTestFixture() :
        config_file_path("/tmp/Llyrdin-configtest.config"),
        hello_world("Hello, World!")
    {
        std::fstream cfg(config_file_path, std::ios::out);

        cfg << "[Data]" << std::endl
            << "stringValue" << " = " << hello_world << std::endl;

        cfg.close();
    }

    ~ConfigTestFixture()
    {
        remove(config_file_path.c_str());
    }

    const std::string config_file_path;
    const std::string hello_world;
};

SUITE(ConfigFileSuite)
{
    TEST_FIXTURE(ConfigTestFixture, parseFile)
    {
        Script::ConfigParser parser;
        CHECK(parser.parse_file(config_file_path));
    }

    TEST_FIXTURE(ConfigTestFixture, readValue)
    {
        Script::ConfigParser parser;
        CHECK(parser.parse_file(config_file_path));
        CHECK_EQUAL(parser.get("stringValue", "Data"), hello_world);
    }

    TEST_FIXTURE(ConfigTestFixture, writeValue)
    {
        Script::ConfigParser parser;
        CHECK(parser.parse_file(config_file_path));
        
        parser.set("stringValue", hello_world, "Data");

        CHECK_EQUAL(parser.get("stringValue", "Data"), hello_world);
    }
}
