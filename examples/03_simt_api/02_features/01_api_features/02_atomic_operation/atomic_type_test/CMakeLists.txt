# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(atomic_type_test LANGUAGES ASC CXX)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU ARCH, e.g. dav-3510")
set(TYPE_KIND "0" CACHE STRING "Data type: 0=float 1=half 2=bfloat16_t 3=half2 4=bfloat16x2_t")
set(SCENARIO_NUM "1" CACHE STRING "Scenario number: 1=GM return 2=GM no-return 3=UB return 4=UB no-return")
set(BLOCK_NUM "48" CACHE STRING "Number of SIMT thread blocks")
set(THREADS_PER_BLOCK "256" CACHE STRING "Number of SIMT threads per block")
set(ITERATIONS "256" CACHE STRING "Atomic add iterations per thread")
option(SKIP_VALIDATION "Skip result validation for msprof op replay profiling" ON)

if(NOT TYPE_KIND MATCHES "^[0-4]$")
    message(FATAL_ERROR "TYPE_KIND must be one of 0..4, but got ${TYPE_KIND}")
endif()

if(NOT SCENARIO_NUM MATCHES "^[1-4]$")
    message(FATAL_ERROR "SCENARIO_NUM must be one of 1..4, but got ${SCENARIO_NUM}")
endif()

add_executable(atomic_type_test
    atomic_type_test.asc
)

target_compile_definitions(atomic_type_test PRIVATE
    TYPE_KIND=${TYPE_KIND}
    SCENARIO_NUM=${SCENARIO_NUM}
    BLOCK_NUM=${BLOCK_NUM}
    THREADS_PER_BLOCK=${THREADS_PER_BLOCK}
    ITERATIONS=${ITERATIONS}
    SKIP_VALIDATION=$<BOOL:${SKIP_VALIDATION}>
)

target_compile_options(atomic_type_test PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES} --enable-simt>
)

target_link_libraries(atomic_type_test PRIVATE
    m
)
