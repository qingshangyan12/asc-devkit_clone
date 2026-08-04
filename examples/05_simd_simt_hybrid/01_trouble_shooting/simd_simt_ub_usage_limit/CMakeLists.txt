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

set(SCENARIO_NUM "1" CACHE STRING "Case number to build (0-3)")
if(NOT SCENARIO_NUM MATCHES "^[0-3]$")
    message(FATAL_ERROR "SCENARIO_NUM must be one of 0, 1, 2, 3.")
endif()

set(CASE_SOURCE "")
if(SCENARIO_NUM STREQUAL "0")
    set(CASE_SOURCE case0_ub_out_of_bounds.asc)
elseif(SCENARIO_NUM STREQUAL "1")
    set(CASE_SOURCE case1_ub_limit.asc)
elseif(SCENARIO_NUM STREQUAL "2")
    set(CASE_SOURCE case2_ub_layout.asc)
elseif(SCENARIO_NUM STREQUAL "3")
    set(CASE_SOURCE case3_out_of_bounds_result.asc)
endif()

set(CASE1_VF_MODE "simd" CACHE STRING "Case 1 VF mode: none, simd, simt, or both")
set_property(CACHE CASE1_VF_MODE PROPERTY STRINGS none simd simt both)
set(CASE1_RESERVED_MODE "default" CACHE STRING "Case 1 reserved-UB mode: default, disable_vf, or disable_all")
set_property(CACHE CASE1_RESERVED_MODE PROPERTY STRINGS default disable_vf disable_all)
option(CASE1_OVER_LIMIT "Add 4 bytes to the Case 1 UB limit to observe the failure result" OFF)
option(CASE0_ENABLE_FIX "Use the fixed Case 0 bias position calculation" OFF)
set(CASE3_ACCESS_REGION "0" CACHE STRING "Case 3 region: 0=static UB, 1=unallocated UB, 2=6KB VF reserve, 3=2KB Ascend C reserve, 4=Data Cache")
if(NOT CASE3_ACCESS_REGION MATCHES "^[0-4]$")
    message(FATAL_ERROR "CASE3_ACCESS_REGION must be one of 0, 1, 2, 3, 4.")
endif()

add_executable(ub_usage_limit
    ${CASE_SOURCE}
)

target_compile_options(ub_usage_limit PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
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
    if(NOT CASE1_VF_MODE MATCHES "^(none|simd|simt|both)$")
        message(FATAL_ERROR "CASE1_VF_MODE must be one of none, simd, simt, both.")
    endif()
    if(NOT CASE1_RESERVED_MODE MATCHES "^(default|disable_vf|disable_all)$")
        message(FATAL_ERROR "CASE1_RESERVED_MODE must be one of default, disable_vf, disable_all.")
    endif()
    if(CASE1_VF_MODE STREQUAL "none" AND NOT CASE1_RESERVED_MODE STREQUAL "default")
        message(WARNING "CASE1_RESERVED_MODE is ignored when CASE1_VF_MODE=none")
    endif()

    set(CASE1_VF_RESERVED_BYTES 0)
    set(CASE1_ASC_RESERVED_BYTES 0)
    set(CASE1_DATA_CACHE_BYTES 0)
    set(CASE1_EXTRA_BYTES 0)
    if(NOT CASE1_VF_MODE STREQUAL "none")
        if(NOT CASE1_RESERVED_MODE STREQUAL "disable_vf" AND NOT CASE1_RESERVED_MODE STREQUAL "disable_all")
            set(CASE1_VF_RESERVED_BYTES 6144)
        endif()
        if(NOT CASE1_RESERVED_MODE STREQUAL "disable_all")
            set(CASE1_ASC_RESERVED_BYTES 2048)
        endif()
        if(CASE1_VF_MODE STREQUAL "simt" OR CASE1_VF_MODE STREQUAL "both")
            set(CASE1_DATA_CACHE_BYTES 32768)
        endif()
    endif()
    if(CASE1_OVER_LIMIT)
        set(CASE1_EXTRA_BYTES 4)
    endif()
    target_compile_definitions(ub_usage_limit PRIVATE
        CASE1_VF_RESERVED_BYTES=${CASE1_VF_RESERVED_BYTES}
        CASE1_ASC_RESERVED_BYTES=${CASE1_ASC_RESERVED_BYTES}
        CASE1_DATA_CACHE_BYTES=${CASE1_DATA_CACHE_BYTES}
        CASE1_EXTRA_BYTES=${CASE1_EXTRA_BYTES}
    )

    if(CASE1_VF_MODE STREQUAL "none")
        target_compile_definitions(ub_usage_limit PRIVATE CASE1_NO_VF=1)
    elseif(CASE1_VF_MODE STREQUAL "simd")
        target_compile_definitions(ub_usage_limit PRIVATE CASE1_SIMD_VF=1)
    elseif(CASE1_VF_MODE STREQUAL "simt")
        target_compile_definitions(ub_usage_limit PRIVATE CASE1_USE_SIMT_VF=1)
    else()
        target_compile_definitions(ub_usage_limit PRIVATE CASE1_USE_BOTH_VF=1)
    endif()
    if(NOT CASE1_VF_MODE STREQUAL "none" AND
       (CASE1_RESERVED_MODE STREQUAL "disable_vf" OR CASE1_RESERVED_MODE STREQUAL "disable_all"))
        target_compile_options(ub_usage_limit PRIVATE
            $<$<COMPILE_LANGUAGE:ASC>:--cce-disable-vf-stack-reserved-ubuf>
        )
    endif()
    if(NOT CASE1_VF_MODE STREQUAL "none" AND CASE1_RESERVED_MODE STREQUAL "disable_all")
        target_compile_options(ub_usage_limit PRIVATE
            $<$<COMPILE_LANGUAGE:ASC>:--cce-disable-asc-reserved-ubuf>
        )
    endif()
elseif(NOT CASE1_VF_MODE STREQUAL "simd" OR NOT CASE1_RESERVED_MODE STREQUAL "default" OR CASE1_OVER_LIMIT)
    message(WARNING "Case 1 options are ignored unless SCENARIO_NUM=1")
endif()

if(SCENARIO_NUM STREQUAL "3")
    target_compile_definitions(ub_usage_limit PRIVATE CASE3_ACCESS_REGION=${CASE3_ACCESS_REGION})
elseif(NOT CASE3_ACCESS_REGION STREQUAL "0")
    message(WARNING "CASE3_ACCESS_REGION is ignored unless SCENARIO_NUM=3")
endif()
