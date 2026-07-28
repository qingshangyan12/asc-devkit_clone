cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")
find_package(ASC REQUIRED)
project(c_api_basic_reg_auxscalar LANGUAGES ASC CXX)

add_executable(c_api_reg_auxscalar reg_auxscalar.asc)
if(NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    message(FATAL_ERROR "${CMAKE_ASC_ARCHITECTURES} is not supported by this sample")
endif()
target_compile_options(c_api_reg_auxscalar PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
