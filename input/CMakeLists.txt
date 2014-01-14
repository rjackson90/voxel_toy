cmake_minimum_required(VERSION 2.8)

# Compile this module's sources into an object library
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(Llyrdin-input SHARED ${SOURCES})
set_target_properties(Llyrdin-input PROPERTIES LINKER_LANGUAGE CXX)

# Compile this modules' unit tests into an object library
file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
add_library(Llyrdin-input-tests SHARED ${TESTS})
set_target_properties(Llyrdin-input-tests PROPERTIES LINKER_LANGUAGE CXX)
