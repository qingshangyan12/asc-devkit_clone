cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

project(kernel_samples)

set(ASC_SRC_FILE erfcx.asc)

if("${CMAKE_ASC_RUN_MODE}" STREQUAL "cpu")
    find_package(tikicpulib REQUIRED)
    set_source_files_properties(${ASC_SRC_FILE} PROPERTIES LANGUAGE CXX)
else()
    find_package(ASC REQUIRED)
    enable_language(ASC CXX)
endif()

add_executable(demo
    ${ASC_SRC_FILE}
)

target_include_directories(demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../impl
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    $<BUILD_INTERFACE:$<$<STREQUAL:${CMAKE_ASC_RUN_MODE},sim>:-g -O0>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${CMAKE_ASC_RUN_MODE},cpu>:-x c++>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${CMAKE_ASC_RUN_MODE},cpu>:-g -O2 -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -Werror>>
)

target_link_libraries(demo PRIVATE
    m
    $<BUILD_INTERFACE:$<$<STREQUAL:${CMAKE_ASC_RUN_MODE},sim>:runtime_camodel>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${CMAKE_ASC_RUN_MODE},sim>:npu_drv>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${CMAKE_ASC_RUN_MODE},cpu>:cpudebug_acl_stub>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${CMAKE_ASC_RUN_MODE},cpu>:tikicpulib::ascend950pr_9599>>
)
