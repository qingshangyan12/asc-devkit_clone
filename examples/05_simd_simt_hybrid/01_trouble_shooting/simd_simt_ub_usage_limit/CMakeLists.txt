# ----------------------------------------------------------------------------------------------------------
# This program is free software, you can redistribute it and/or modify it.
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This file is a part of the CANN Open Software.
# Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")

set(SCENARIO_NUM "1" CACHE STRING "Case number to build (0-4)")
if(NOT SCENARIO_NUM MATCHES "^[0-4]$")
    message(FATAL_ERROR "SCENARIO_NUM must be one of 0, 1, 2, 3, 4.")
endif()

set(UB_BOUNDARY_STATIC_BYTES "253920" CACHE STRING "Static UB array size used by the Case 1 limit study")
option(UB_BOUNDARY_NO_SIMD_VF "Fill static UB directly in MainScalar without calling a SIMD VF" OFF)
option(UB_BOUNDARY_DISABLE_RESERVED "Disable the VF stack reserved UB region" OFF)
set(CASE2_UB_BYTES "221152" CACHE STRING "Static or dynamic UB size used by Case 2")
option(CASE2_USE_STATIC_UB "Use static UB instead of dynamic UB in Case 2" OFF)
set(CASE3_DYNAMIC_BYTES "213920" CACHE STRING "Dynamic UB size used by Case 3")
option(CASE0_ENABLE_FIX "Block the invalid Case 0 static UB access with a bounds check" OFF)
set(CASE4_ACCESS_REGION "0" CACHE STRING "Case 4 region: 0=static UB, 1=unallocated UB, 2=6KB VF stack, 3=2KB Ascend C reserve, 4=Data Cache")
if(NOT CASE4_ACCESS_REGION MATCHES "^[0-4]$")
    message(FATAL_ERROR "CASE4_ACCESS_REGION must be one of 0, 1, 2, 3, 4.")
endif()

add_executable(ub_usage_limit
    ub_usage_limit.asc
)

target_compile_options(ub_usage_limit PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
target_compile_definitions(ub_usage_limit PRIVATE SCENARIO_NUM=${SCENARIO_NUM})

if(CASE0_ENABLE_FIX)
    if(NOT SCENARIO_NUM STREQUAL "0")
        message(WARNING "CASE0_ENABLE_FIX is ignored unless SCENARIO_NUM=0")
    else()
        target_compile_definitions(ub_usage_limit PRIVATE CASE0_ENABLE_FIX=1)
    endif()
endif()

target_link_libraries(ub_usage_limit PRIVATE
    m
)

if(SCENARIO_NUM STREQUAL "1")
    target_compile_definitions(ub_usage_limit PRIVATE
        UB_BOUNDARY_STATIC_BYTES=${UB_BOUNDARY_STATIC_BYTES}
    )
    if(UB_BOUNDARY_NO_SIMD_VF)
        target_compile_definitions(ub_usage_limit PRIVATE UB_BOUNDARY_NO_SIMD_VF=1)
    endif()
    if(UB_BOUNDARY_DISABLE_RESERVED)
        target_compile_options(ub_usage_limit PRIVATE
            $<$<COMPILE_LANGUAGE:ASC>:--cce-disable-vf-stack-reserved-ubuf>
        )
    endif()
elseif(UB_BOUNDARY_NO_SIMD_VF OR UB_BOUNDARY_DISABLE_RESERVED)
    message(WARNING "UB boundary options are ignored unless SCENARIO_NUM=1")
endif()

if(SCENARIO_NUM STREQUAL "2")
    target_compile_definitions(ub_usage_limit PRIVATE CASE2_UB_BYTES=${CASE2_UB_BYTES})
    if(CASE2_USE_STATIC_UB)
        target_compile_definitions(ub_usage_limit PRIVATE CASE2_USE_STATIC_UB=1)
    endif()
elseif(CASE2_USE_STATIC_UB)
    message(WARNING "CASE2_USE_STATIC_UB is ignored unless SCENARIO_NUM=2")
endif()

if(SCENARIO_NUM STREQUAL "3")
    target_compile_definitions(ub_usage_limit PRIVATE CASE3_DYNAMIC_BYTES=${CASE3_DYNAMIC_BYTES})
endif()

if(SCENARIO_NUM STREQUAL "4")
    target_compile_definitions(ub_usage_limit PRIVATE CASE4_ACCESS_REGION=${CASE4_ACCESS_REGION})
elseif(NOT CASE4_ACCESS_REGION STREQUAL "0")
    message(WARNING "CASE4_ACCESS_REGION is ignored unless SCENARIO_NUM=4")
endif()
