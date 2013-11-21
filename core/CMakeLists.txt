cmake_minimum_required(VERSION 2.8)

# Compile this module's sources into an object library
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(Llyrdin-core SHARED ${SOURCES})

# Compile this modules' unit tests into an object library
file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
add_library(Llyrdin-core-tests SHARED ${TESTS})

# Compile this module's Python API into shared libraries
file(GLOB EXTENSIONS ${CMAKE_CURRENT_SOURCE_DIR}/api/*.cpp)
foreach(EXTENSION ${EXTENSIONS})
    get_filename_component(EXTENSION_ROOT ${EXTENSION} NAME_WE)
    add_library(${EXTENSION_ROOT} SHARED ${EXTENSION})
    target_link_libraries(${EXTENSION_ROOT} ${MODULE_LIBS} ${LIB_DIRS})
    set_target_properties(${EXTENSION_ROOT} PROPERTIES PREFIX "" LINKER_LANGUAGE CXX)
endforeach()
