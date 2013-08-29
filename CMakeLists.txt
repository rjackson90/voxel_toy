# Declare minimum CMake version, project name
cmake_minimum_required(VERSION 2.8)
project(GLDemo)

# Set appropriate flags for g++
if(CMAKE_COMPILER_IS_GNUCXX)
    message("Using GNU C++ compiler")
    set(FLAGS "-Wall -Wextra -Werror -pedantic -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${FLAGS} -ggdb -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${FLAGS} -O3")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -ggdb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
else(CMAKE_COMPILER_IS_GNUCXX)
    message("[WARN] Target compiler is NOT a gnu compiler!")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Built artifacts should all be in build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GLDemo_BINARY_DIR}/bin)

# Glob together a list of the common source files between executable and test builds
file(GLOB SOURCES src/*.cpp)
list(REMOVE_ITEM SOURCES ${GLDemo_SOURCE_DIR}/src/main.cpp)

# Glob together a list of tests
file(GLOB TESTS tests/*.cpp)

# Tell CMake where the includes are
include_directories(${GLDemo_SOURCE_DIR}/include)

# Compile the two sets of source files
add_library(DCClasses OBJECT ${SOURCES})
add_executable(DeepCore $<TARGET_OBJECTS:DCClasses> src/main.cpp)
add_executable(DeepCoreTests $<TARGET_OBJECTS:DCClasses> ${TESTS})

# Link the source objects to finish building the executables
set(LIBRARIES GLEW GLU GL X11)
target_link_libraries(DeepCore ${LIBRARIES})
target_link_libraries(DeepCoreTests ${LIBRARIES} UnitTest++)

# Run the tests after they (successfully) build
add_custom_target(tests ALL DeepCoreTests 
    COMMENT "############### Running Unit Tests ###############")
add_dependencies(tests DeepCoreTests)

