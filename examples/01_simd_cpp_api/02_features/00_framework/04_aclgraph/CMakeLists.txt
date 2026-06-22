cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")

find_package(ASC REQUIRED)

project(aclgraph_add_demo LANGUAGES ASC CXX)

add_executable(demo
    add.asc
)

set(SCENARIO_NUM "1" CACHE STRING "Scenario number: 1 - single stream, 2 - double stream")
if(NOT SCENARIO_NUM MATCHES "^[1-2]$")
    message(FATAL_ERROR "SCENARIO_NUM must be 1 or 2, but got ${SCENARIO_NUM}")
endif()

add_definitions(-DSCENARIO_NUM=${SCENARIO_NUM})
