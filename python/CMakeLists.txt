cmake_minimum_required(VERSION 2.8)

# Compile this module's sources into an object library
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(Llyrdin-python OBJECT ${SOURCES})

# Compile this modules' unit tests into an object library
file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
add_library(Llyrdin-python-tests OBJECT ${TESTS})
