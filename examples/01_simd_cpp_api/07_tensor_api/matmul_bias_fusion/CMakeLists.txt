cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")
option(CANN_ASC_USE_EXPERIMENTAL "Enable experimental CANN ASC APIs" OFF)

if(NOT CMAKE_ASC_RUN_MODE STREQUAL "npu" AND NOT CMAKE_ASC_RUN_MODE STREQUAL "sim")
    message(FATAL_ERROR "CMAKE_ASC_RUN_MODE must be 'npu' or 'sim'")
endif()
if(NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    message(FATAL_ERROR "matmul_bias_fusion only supports dav-3510")
endif()
if(NOT CANN_ASC_USE_EXPERIMENTAL)
    message(FATAL_ERROR "matmul_bias_fusion requires CANN_ASC_USE_EXPERIMENTAL=ON")
endif()

find_package(ASC REQUIRED)
project(matmul_bias_fusion LANGUAGES ASC CXX)

add_executable(demo matmul_bias_fusion.asc)
if(CMAKE_ASC_RUN_MODE STREQUAL "sim")
    set(SAMPLE_M 32)
    set(SAMPLE_K 32)
    set(SAMPLE_N 32)
    target_compile_definitions(demo PRIVATE MATMUL_BIAS_FUSION_SIM_VALIDATION=1)
else()
    set(SAMPLE_M 1024)
    set(SAMPLE_K 256)
    set(SAMPLE_N 1024)
endif()
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sample_config.json"
    CONTENT "{\"m\": ${SAMPLE_M}, \"k\": ${SAMPLE_K}, \"n\": ${SAMPLE_N}}\n")
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    $<$<AND:$<COMPILE_LANGUAGE:ASC>,$<BOOL:${CANN_ASC_USE_EXPERIMENTAL}>>:-DCANN_ASC_USE_EXPERIMENTAL>
)
