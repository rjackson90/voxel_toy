# Declare minimum CMake version, project name
cmake_minimum_required(VERSION 2.8)
project(GLDemo CXX)

# List of modules in the project
set(MODULES core physics rendering input)

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

# Binary artifacts should all be in build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GLDemo_BINARY_DIR}/bin)

# Tell CMake where the includes are
foreach( MODULE ${MODULES})
    include_directories("${MODULE}/include/")
endforeach()

# Create the special "data-path" header
foreach( MODULE ${MODULES})
    set( "${MODULE}_data_path" "${CMAKE_SOURCE_DIR}/${MODULE}/data/" )
endforeach()
set( "rendering_shaders_path" "${CMAKE_SOURCE_DIR}/rendering/shaders/" )
configure_file( exec/include/data_paths.h.in ${CMAKE_BINARY_DIR}/exec/include/data_paths.h )
include_directories( ${CMAKE_BINARY_DIR}/exec/include/ )

# Compile each module, get the name of each library
foreach( MODULE ${MODULES})
    add_subdirectory(${MODULE})
    set( MODULE_LIBS ${MODULE_LIBS} "$<TARGET_OBJECTS:GLDemo-${MODULE}>" )
    set( MODULE_TEST_LIBS ${MODULE_TEST_LIBS} "$<TARGET_OBJECTS:GLDemo-${MODULE}-tests>" )
endforeach()

# Compile the main executable
add_executable(DeepCore ${MODULE_LIBS} exec/src/main.cpp)
add_executable(DeepCoreTests ${MODULE_LIBS} ${MODULE_TEST_LIBS} exec/src/tests.cpp)

# Find required libraries
include(FindPkgConfig)
pkg_check_modules(SYSTEM_LIBS REQUIRED glew glu gl sdl2)
include_directories(${SYSTEM_LIBS_INCLUDE_DIRS})

# Link executables using found system libraries
target_link_libraries(DeepCore ${SYSTEM_LIBS_LIBRARIES})
target_link_libraries(DeepCoreTests ${SYSTEM_LIBS_LIBRARIES} UnitTest++)


# Run the tests after they (successfully) build
add_custom_target(tests ALL DeepCoreTests 
    COMMENT "############### Running Unit Tests ###############")
add_dependencies(tests DeepCoreTests)

