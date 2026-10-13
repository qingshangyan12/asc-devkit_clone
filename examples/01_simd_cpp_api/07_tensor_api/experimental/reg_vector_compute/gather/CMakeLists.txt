cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")
option(CANN_ASC_USE_EXPERIMENTAL "Enable experimental CANN ASC APIs" ON)

find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)

if(NOT CANN_ASC_USE_EXPERIMENTAL)
    message(FATAL_ERROR "This sample requires -DCANN_ASC_USE_EXPERIMENTAL=ON")
endif()

get_filename_component(ASC_DEVKIT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.." ABSOLUTE)
add_executable(demo gather.asc)
target_include_directories(demo BEFORE PRIVATE
    "${ASC_DEVKIT_ROOT}/include"
    "${ASC_DEVKIT_ROOT}/include/basic_api"
    "${ASC_DEVKIT_ROOT}/include/utils"
    "${ASC_DEVKIT_ROOT}"
)
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    $<$<COMPILE_LANGUAGE:ASC>:-DCANN_ASC_USE_EXPERIMENTAL>
)
