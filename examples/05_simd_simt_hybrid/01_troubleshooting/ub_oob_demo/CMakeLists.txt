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

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")
set(CASE_NUM "1" CACHE STRING "OOB symptom tier: 1=silent wrong data, 2=reserved-area overrun, 3=Data Cache fault")
option(GUARD_BAND "Insert a sentinel band after the scratch buffer to measure the overflow" OFF)
option(BOUND_PRINTF "Print the first out-of-bounds index/capacity pair" OFF)
option(FIX_OOB "Size the scratch buffer for the real warp count (applies the fix)" OFF)

find_package(ASC REQUIRED)

project(ub_oob_demo LANGUAGES ASC CXX)

add_executable(demo
    ub_oob_demo.asc
)

message(STATUS "[INFO] CASE_NUM is ${CASE_NUM}")
message(STATUS "[INFO] GUARD_BAND is ${GUARD_BAND}")
message(STATUS "[INFO] BOUND_PRINTF is ${BOUND_PRINTF}")
message(STATUS "[INFO] FIX_OOB is ${FIX_OOB}")

target_compile_definitions(demo PRIVATE
    CASE_NUM=${CASE_NUM}
    GUARD_BAND=$<BOOL:${GUARD_BAND}>
    BOUND_PRINTF=$<BOOL:${BOUND_PRINTF}>
    FIX_OOB=$<BOOL:${FIX_OOB}>
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
