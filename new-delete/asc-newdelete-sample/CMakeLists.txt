# ----------------------------------------------------------------------------------------------------------
# This sample is adapted from NVIDIA CUDA Samples cpp/3_CUDA_Features/newdelete.
# It demonstrates C++ new/delete and placement new in an Ascend C SIMT kernel.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(newdelete LANGUAGES ASC CXX)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU ARCH, e.g. dav-3510")

add_executable(demo
    newdelete.asc
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES} --enable-simt>
)
