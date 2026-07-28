cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")
set(SCENARIO_NUM "1" CACHE STRING "Sample scenario: 1, 2, or 3")
find_package(ASC REQUIRED)
project(c_api_basic_copy_ub2ub LANGUAGES ASC CXX)

add_executable(c_api_copy_ub2ub copy_ub2ub.asc)
if(NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    message(FATAL_ERROR "${CMAKE_ASC_ARCHITECTURES} is not supported by this sample")
endif()
if(NOT SCENARIO_NUM MATCHES "^[123]$")
    message(FATAL_ERROR "SCENARIO_NUM must be 1, 2, or 3")
endif()
target_compile_definitions(c_api_copy_ub2ub PRIVATE SCENARIO_NUM=${SCENARIO_NUM})
target_compile_options(c_api_copy_ub2ub PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
