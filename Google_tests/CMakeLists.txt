cmake_minimum_required(VERSION 3.14)
project(Google_tests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Add Google Test
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Add test executable
add_executable(Matrix_Tests MatrixTest.cpp ../Matrix.cpp)
add_executable(Node_Tests NodeTest.cpp ../Node.cpp ../Matrix.cpp)
add_executable(Graph_Tests GraphTest.cpp ../Graph.cpp ../Node.cpp ../Matrix.cpp ../ArbAssignments.cpp)
# Link Google Test libraries
target_link_libraries(Matrix_Tests gtest gtest_main)
target_link_libraries(Node_Tests gtest gtest_main)
target_link_libraries(Graph_Tests gtest gtest_main)

# Tell CTest (and CLion) how to discover the tests
include(GoogleTest)
gtest_discover_tests(Matrix_Tests)
gtest_discover_tests(Node_Tests)
gtest_discover_tests(Graph_Tests)