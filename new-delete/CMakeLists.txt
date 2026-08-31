# ----------------------------------------------------------------------------------------------------------
# Minimal probe for device-side new/delete on Ascend C SIMT.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(newdelete_dpp_probe LANGUAGES ASC CXX)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU ARCH, e.g. dav-3510")

add_executable(aligned_probe
    aligned_probe.asc
)

target_compile_options(aligned_probe PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES} --enable-simt>
)
