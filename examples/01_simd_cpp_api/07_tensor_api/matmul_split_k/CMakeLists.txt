cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

if(NOT CMAKE_ASC_RUN_MODE STREQUAL "npu" AND NOT CMAKE_ASC_RUN_MODE STREQUAL "sim")
    message(FATAL_ERROR "CMAKE_ASC_RUN_MODE must be 'npu' or 'sim'")
endif()
if(NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    message(FATAL_ERROR "matmul_split_k only supports dav-3510")
endif()

find_package(ASC REQUIRED)
project(matmul_split_k LANGUAGES ASC CXX)

add_executable(demo matmul_split_k.asc)
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
