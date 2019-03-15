cmake_minimum_required(VERSION 3.10)
project(IW1)

set(CMAKE_CXX_STANDARD 11)

add_executable(IW1 main.cpp IGraph.h CMatrixGraph.h CListGraph.h CSetGraph.h CArcGraph.h CMatrixGraph.cpp CListgraph.cpp CArcGraph.cpp CSetGraph.cpp)