cmake_minimum_required(VERSION 3.31)
project(feature_assigner)

set(CMAKE_CXX_STANDARD 17)

add_executable(feature_assigner
        Framework.cpp
        Framework.h
        ArbAssignments.cpp
        ArbAssignments.h
        Graph.cpp
        Graph.h
        Matrix.cpp
        Matrix.h
        Node.cpp
        Node.h
)

#add_subdirectory(Google_tests)

