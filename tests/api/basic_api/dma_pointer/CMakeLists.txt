# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software under CANN Open Software License Agreement Version 2.0.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND.
# See LICENSE in the root of the software repository for the full text of the License.

cmake_minimum_required(VERSION 3.16)
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture for the DMA runtime test")
find_package(ASC REQUIRED)
project(dma_pointer_runtime LANGUAGES ASC CXX)

get_filename_component(DEVKIT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)
add_executable(dma_pointer_runtime runtime_smoke.asc)
set(DMA_RUNTIME_TARGETS dma_pointer_runtime)
if(CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    add_executable(dma_pointer_cache runtime_cache.asc)
    add_executable(dma_pointer_l1toub runtime_l1toub.asc)
    list(APPEND DMA_RUNTIME_TARGETS dma_pointer_cache dma_pointer_l1toub)
endif()
foreach(dma_test IN LISTS DMA_RUNTIME_TARGETS)
    set_target_properties(${dma_test} PROPERTIES CXX_STANDARD 17)
    target_compile_definitions(${dma_test} PRIVATE ASCENDC_DUMP=0)
    target_compile_options(${dma_test} PRIVATE
        --npu-arch=${CMAKE_ASC_ARCHITECTURES} -std=c++17
        "-iquote${DEVKIT_ROOT}"
        "-iquote${DEVKIT_ROOT}/include"
        "-iquote${DEVKIT_ROOT}/impl"
        "-iquote${DEVKIT_ROOT}/include/basic_api"
        "-iquote${DEVKIT_ROOT}/impl/basic_api")
    target_include_directories(${dma_test} PRIVATE
        "${DEVKIT_ROOT}" "${DEVKIT_ROOT}/include" "${DEVKIT_ROOT}/impl")
endforeach()
