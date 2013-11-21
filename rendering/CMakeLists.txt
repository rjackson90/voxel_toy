cmake_minimum_required(VERSION 2.8)

# Compile this module's sources into an object library
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(Llyrdin-rendering SHARED ${SOURCES})
set_target_properties(Llyrdin-rendering PROPERTIES LINKER_LANGUAGE CXX)

# Compile this modules' unit tests into an object library
file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
add_library(Llyrdin-rendering-tests SHARED ${TESTS})
set_target_properties(Llyrdin-rendering-tests PROPERTIES LINKER_LANGUAGE CXX)
