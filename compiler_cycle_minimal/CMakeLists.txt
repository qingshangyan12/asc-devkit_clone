cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture")

find_package(ASC REQUIRED)

project(compiler_cycle_minimal LANGUAGES ASC CXX)

add_executable(demo
    read_cycles.asc
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    $<$<COMPILE_LANGUAGE:ASC>:-g>
    $<$<COMPILE_LANGUAGE:ASC>:-O2>
    $<$<COMPILE_LANGUAGE:ASC>:-save-temps=obj>
)

target_include_directories(demo BEFORE PRIVATE
    /tmp/asc-devkit-timestamp-install/cann/aarch64-linux/tikcpp/tikcfw
    /tmp/asc-devkit-timestamp-install/cann/aarch64-linux/asc/include
    /usr/local/Ascend/cann-9.1.0/include
)

target_link_directories(demo PRIVATE
    /usr/local/Ascend/cann-9.1.0/lib64
)
