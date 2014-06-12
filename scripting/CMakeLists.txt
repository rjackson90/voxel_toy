cmake_minimum_required(VERSION 2.8)

# Compile this module's sources into a shared library
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(Llyrdin-scripting SHARED ${SOURCES})
set_target_properties(Llyrdin-scripting PROPERTIES LINKER_LANGUAGE CXX)

# Compile this modules' unit tests into a shared library
file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
add_library(Llyrdin-scripting-tests SHARED ${TESTS})
set_target_properties(Llyrdin-scripting-tests PROPERTIES LINKER_LANGUAGE CXX)

# Compile this module's Python API into shared libraries
file(GLOB EXTENSIONS ${CMAKE_CURRENT_SOURCE_DIR}/api/*.cpp)
foreach(EXTENSION ${EXTENSIONS})
    get_filename_component(EXTENSION_ROOT ${EXTENSION} NAME_WE)
    add_library(${EXTENSION_ROOT} SHARED ${EXTENSION})
    target_link_libraries(${EXTENSION_ROOT} ${MODULE_LIBS} ${LIB_DIRS})
    set_target_properties(${EXTENSION_ROOT} PROPERTIES PREFIX "" LINKER_LANGUAGE CXX)
endforeach()
