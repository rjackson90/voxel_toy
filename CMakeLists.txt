# Declare minimum CMake version, project name
cmake_minimum_required(VERSION 2.8.8)
project(Llyrdin CXX C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/exec/cmake_modules")

# List of modules in the project
set(MODULES core physics rendering python input)

# Set appropriate flags for g++
if(CMAKE_COMPILER_IS_GNUCXX)
    message("Using GNU C++ compiler")
    set(FLAGS "-Wall -Wextra -Werror -pedantic -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${FLAGS} -ggdb -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${FLAGS} -O3")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -ggdb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
else(CMAKE_COMPILER_IS_GNUCXX)
    message(SEND_ERROR "[WARN] Target compiler is NOT a gnu compiler!")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Binary artifacts should all be in build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Llyrdin_BINARY_DIR}/bin)

# Tell CMake where the includes (and external library binaries) are
foreach( MODULE ${MODULES})
    include_directories("${MODULE}/include/")
endforeach()

set(LIB_DIRS "")
# ... for Python
set(Python_ADDITIONAL_VERSIONS "2.7")
find_package(PythonLibs)
if(${PYTHONLIBS_FOUND})
    message(STATUS "Found Python Libraries, version ${PYTHONLIBS_VERSION_STRING}")
    include_directories(${PYTHON_INCLUDE_DIRS})
    set(LIB_DIRS ${LIB_DIRS} ${PYTHON_LIBRARIES})
else(${PYTHONLIBS_FOUND})
    message(SEND_ERROR "Python libraries not found")
endif()

# ... for GLEW
find_package(GLEW)
if(${GLEW_FOUND})
    message(STATUS "Found GLEW")
    include_directories(${GLEW_INCLUDE_DIRS})
    set(LIB_DIRS ${LIB_DIRS} ${GLEW_LIBRARIES})
else(${GLEW_FOUND})
    message(SEND_ERROR "GLEW not found")
endif(${GLEW_FOUND})

# ... for OpenGL
find_package(OpenGL)
if(${OPENGL_FOUND})
    message(STATUS "Found OpenGL")
    include_directories(${OPENGL_INCLUDE_DIR})
    set(LIB_DIRS ${LIB_DIRS} ${OPENGL_gl_LIBRARY})
else(${OPENGL_FOUND})
    message(SEND_ERROR "OpenGL not found")
endif(${OPENGL_FOUND})

# ... for SDL 2.0
find_package(SDL2)
if(${SDL2_FOUND})
    message(STATUS "Found SDL2")
    include_directories(${SDL2_INCLUDE_DIR})
    set(LIB_DIRS ${LIB_DIRS} ${SDL2_LIBRARY})
else(${SDL2_FOUND})
    message(SEND_ERROR "SDL2 not found")
endif(${SDL2_FOUND})

# ... for Boost 
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.49.0 COMPONENTS python)
if(${Boost_FOUND})
    include_directories(${Boost_INCLUDE_DIRS})
    set(LIB_DIRS ${LIB_DIRS} ${Boost_LIBRARIES})
else(${Boost_FOUND})
    message(SEND_ERROR "One or more Boost libraries not found")
endif(${Boost_FOUND})

# Create the special "data-path" header
foreach( MODULE ${MODULES})
    set( "${MODULE}_data_path" "${CMAKE_SOURCE_DIR}/${MODULE}/data/" )
endforeach()
set( "rendering_shaders_path" "${CMAKE_SOURCE_DIR}/rendering/shaders/" )
set( "python_path" "${CMAKE_SOURCE_DIR}/python/" )
configure_file( exec/include/data_paths.h.in ${CMAKE_BINARY_DIR}/exec/include/data_paths.h )
include_directories( ${CMAKE_BINARY_DIR}/exec/include/ )

# Compile each module, get the name of each library
foreach( MODULE ${MODULES})
    add_subdirectory(${MODULE})
    set( MODULE_LIBS ${MODULE_LIBS} "$<TARGET_OBJECTS:Llyrdin-${MODULE}>" )
    set( MODULE_TEST_LIBS ${MODULE_TEST_LIBS} "$<TARGET_OBJECTS:Llyrdin-${MODULE}-tests>" )
endforeach()

# Compile the main executable
add_executable(Llyrdin ${MODULE_LIBS} exec/src/main.cpp)
add_executable(LlyrdinTests ${MODULE_LIBS} ${MODULE_TEST_LIBS} exec/src/tests.cpp)

# Link executables using found system libraries
target_link_libraries(Llyrdin ${LIB_DIRS})
target_link_libraries(LlyrdinTests ${LIB_DIRS} UnitTest++)


# Run the tests after they (successfully) build
add_custom_target(tests ALL LlyrdinTests 
    COMMENT "############### Running Unit Tests ###############")
add_dependencies(tests LlyrdinTests)

