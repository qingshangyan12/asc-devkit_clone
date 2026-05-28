cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-2201")

find_package(ASC REQUIRED)

project(aclgraph_double_stream_demo LANGUAGES ASC CXX)

add_executable(demo
    add.asc
)

