cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")
set(CMAKE_ASC_STANDARD 17 CACHE STRING "ASC language standard")

find_package(ASC REQUIRED)
project(adaptive_max_pool3d_grad_simd_simt_hybrid LANGUAGES ASC CXX)

add_executable(demo adaptive_max_pool3d_grad.asc)

message(STATUS "[INFO] Runtime mode is selected by ./demo <simd|scalar|hybrid|hybrid_ub> [profile]")

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)

target_link_libraries(demo PRIVATE
    platform
    tiling_api
)
