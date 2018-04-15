cmake_minimum_required(VERSION 3.9)
project(TriangleCountingAPI)

set(CMAKE_CXX_STANDARD 14)

if (WIN32)
    #REPLACE PATH
    include_directories("E:\\Downloads\\boost_1_65_1\\boost_1_65_1")
endif (WIN32)


add_definitions( "-m64" )

add_executable(TriangleCountingAPI main.cpp gps_post_stream.cpp gps_post_stream.h adjacency_matrix_graph.cpp adjacency_matrix_graph.h adjacency_list_graph.cpp adjacency_list_graph.h sampler.cpp sampler.h sampler_edge_array.cpp sampler_edge_array.h)