# Tell CMake about the include directory
include_directories(${GLDemo_SOURCE_DIR}/include)

file(GLOB SOURCES *.cpp)
list(REMOVE_ITEM SOURCES ${GLDemo_SOURCE_DIR}/src/main.cpp)

# Build the main executable and link with dependencies
add_executable(DeepCore ${SOURCES} main.cpp)
target_link_libraries(DeepCore GLEW GLU GL X11)
