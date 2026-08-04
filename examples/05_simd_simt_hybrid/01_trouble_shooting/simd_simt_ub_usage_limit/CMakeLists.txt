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

set(UB_LIMIT_BYTES "" CACHE STRING "UB array size used by Case 1")
option(UB_LIMIT_WITH_SIMT_VF "Validate the UB limit with a SIMT VF" OFF)
option(UB_LIMIT_NO_SIMD_VF "Do not call a SIMD VF in the non-SIMT scenario" OFF)
option(UB_LIMIT_DISABLE_VF_RESERVED "Disable the VF reserved UB region" OFF)
option(UB_LIMIT_DISABLE_ASC_RESERVED "Disable the Ascend C API reserved UB region" OFF)
option(CASE0_ENABLE_FIX "Block the invalid Case 0 static UB access with a bounds check" OFF)
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
    if(UB_LIMIT_BYTES STREQUAL "")
        if(UB_LIMIT_WITH_SIMT_VF)
            set(UB_LIMIT_BYTES_EFFECTIVE "221184")
        else()
            set(UB_LIMIT_BYTES_EFFECTIVE "253952")
        endif()
    else()
        set(UB_LIMIT_BYTES_EFFECTIVE "${UB_LIMIT_BYTES}")
    endif()
    target_compile_definitions(ub_usage_limit PRIVATE UB_LIMIT_BYTES=${UB_LIMIT_BYTES_EFFECTIVE})
    if(UB_LIMIT_WITH_SIMT_VF)
        target_compile_definitions(ub_usage_limit PRIVATE UB_LIMIT_WITH_SIMT_VF=1)
    endif()
    if(UB_LIMIT_NO_SIMD_VF)
        if(UB_LIMIT_WITH_SIMT_VF)
            message(FATAL_ERROR "UB_LIMIT_NO_SIMD_VF and UB_LIMIT_WITH_SIMT_VF cannot both be ON")
        endif()
        target_compile_definitions(ub_usage_limit PRIVATE UB_LIMIT_NO_SIMD_VF=1)
    endif()
    if(UB_LIMIT_DISABLE_VF_RESERVED)
        target_compile_definitions(ub_usage_limit PRIVATE UB_LIMIT_DISABLE_VF_RESERVED=1)
        target_compile_options(ub_usage_limit PRIVATE
            $<$<COMPILE_LANGUAGE:ASC>:--cce-disable-vf-stack-reserved-ubuf>
        )
    endif()
    if(UB_LIMIT_DISABLE_ASC_RESERVED)
        target_compile_definitions(ub_usage_limit PRIVATE UB_LIMIT_DISABLE_ASC_RESERVED=1)
        target_compile_options(ub_usage_limit PRIVATE
            $<$<COMPILE_LANGUAGE:ASC>:--cce-disable-asc-reserved-ubuf>
        )
    endif()
elseif(UB_LIMIT_WITH_SIMT_VF OR UB_LIMIT_NO_SIMD_VF OR
       UB_LIMIT_DISABLE_VF_RESERVED OR UB_LIMIT_DISABLE_ASC_RESERVED)
    message(WARNING "UB limit options are ignored unless SCENARIO_NUM=1")
endif()

if(SCENARIO_NUM STREQUAL "3")
    target_compile_definitions(ub_usage_limit PRIVATE CASE3_ACCESS_REGION=${CASE3_ACCESS_REGION})
elseif(NOT CASE3_ACCESS_REGION STREQUAL "0")
    message(WARNING "CASE3_ACCESS_REGION is ignored unless SCENARIO_NUM=3")
endif()
