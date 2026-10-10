cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")
set(SCENARIO_NUM "1" CACHE STRING "Scenario: 1=unaligned copy, 2=datablock reduce max")
option(CANN_ASC_USE_EXPERIMENTAL "Enable experimental CANN ASC APIs" OFF)

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

if(NOT SCENARIO_NUM EQUAL 1 AND NOT SCENARIO_NUM EQUAL 2)
    message(FATAL_ERROR "SCENARIO_NUM must be 1 or 2, got: ${SCENARIO_NUM}")
endif()

add_executable(demo
    reg_load_store_unalign.asc
)

target_compile_definitions(demo PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    $<$<AND:$<COMPILE_LANGUAGE:ASC>,$<BOOL:${CANN_ASC_USE_EXPERIMENTAL}>>:-DCANN_ASC_USE_EXPERIMENTAL>
)

message(STATUS "[INFO] SCENARIO_NUM is ${SCENARIO_NUM}")

enable_testing()
add_test(NAME reg_load_store_unalign_scenario_${SCENARIO_NUM} COMMAND demo)
