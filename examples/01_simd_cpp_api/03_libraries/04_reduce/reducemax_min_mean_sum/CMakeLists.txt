# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.16)

if(DEFINED RUN_MODE)
    set(CMAKE_ASC_RUN_MODE ${RUN_MODE} CACHE STRING "Alias of RUN_MODE")
endif()

if(DEFINED NPU_ARCH)
    set(CMAKE_ASC_ARCHITECTURES ${NPU_ARCH} CACHE STRING "Alias of NPU_ARCH")
endif()

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

# ======================================================================================
# 场景配置（通过编译时宏传递）
# SCENARIO_NUM: 场景编号
#   1 - ReduceMax：输入shape [136, 32]，输出shape [136]，Pattern=RA
#   2 - ReduceMin：输入shape [136, 32]，输出shape [136]，Pattern=RA
#   3 - ReduceMean：输入shape [136, 32]，输出shape [136]，Pattern=RA
#   4 - ReduceSum：输入shape [136, 32]，输出shape [136]，Pattern=RA
# ======================================================================================
set(SCENARIO_NUM "1" CACHE STRING "场景编号 (1=ReduceMax, 2=ReduceMin, 3=ReduceMean, 4=ReduceSum)")

add_definitions(-DSCENARIO_NUM=${SCENARIO_NUM})

add_executable(demo
    reducemax_min_mean_sum.asc
)

set(NPU_ARCH "dav-2201" CACHE STRING "NPU ARCH, e.g. dav-2201, dav-3510")

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    unified_dlog
    dl
    m
    graph_base
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${NPU_ARCH}>
)
