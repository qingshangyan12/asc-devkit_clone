# ----------------------------------------------------------------------------------------------------------
# Ascend C Softmax 算子实践教程 - 多版本编译
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    softmax.asc
)

set(SOFTMAX_COMMON_LIBS  m)
target_link_libraries(demo PRIVATE ${SOFTMAX_COMMON_LIBS})

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
