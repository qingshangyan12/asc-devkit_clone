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

project(atomic_add_perf LANGUAGES ASC CXX)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU ARCH, e.g. dav-3510")
set(SCENARIO_NUM "1" CACHE STRING "Scenario number (1-12)")
set(BLOCK_NUM "" CACHE STRING "Number of SIMT thread blocks; empty uses the scenario default")
set(THREADS_PER_BLOCK "" CACHE STRING "Number of SIMT threads per block; empty uses the scenario default")
set(TARGET_COUNT "" CACHE STRING "Case 4/5 dense target pool size; empty uses the source default (1024). Shrink to intensify contention, enlarge to relax it")
option(SKIP_VALIDATION "Skip result validation for replay-based profiling tools" OFF)

if(NOT SCENARIO_NUM MATCHES "^([1-9]|1[0-2])$")
    message(FATAL_ERROR "SCENARIO_NUM must be one of 1..12, but got ${SCENARIO_NUM}")
endif()

add_executable(atomic_add_perf
    atomic_add_perf.asc
)

target_compile_definitions(atomic_add_perf PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
    SKIP_VALIDATION=$<BOOL:${SKIP_VALIDATION}>
)

# BLOCK_NUM / THREADS_PER_BLOCK are optional overrides. When left empty the
# source picks a per-scenario default that reproduces README.md.
if(NOT BLOCK_NUM STREQUAL "")
    target_compile_definitions(atomic_add_perf PRIVATE BLOCK_NUM=${BLOCK_NUM})
endif()
if(NOT THREADS_PER_BLOCK STREQUAL "")
    target_compile_definitions(atomic_add_perf PRIVATE THREADS_PER_BLOCK=${THREADS_PER_BLOCK})
endif()
# TARGET_COUNT sizes the Case 4/5 dense target pool. When left empty the source
# picks the default (1024). Shrink to intensify contention, enlarge to relax it.
if(NOT TARGET_COUNT STREQUAL "")
    target_compile_definitions(atomic_add_perf PRIVATE TARGET_COUNT=${TARGET_COUNT})
endif()

target_compile_options(atomic_add_perf PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES} --enable-simt>
)

target_link_libraries(atomic_add_perf PRIVATE
    m
)
