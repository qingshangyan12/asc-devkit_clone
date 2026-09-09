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

project(kernel_samples LANGUAGES ASC CXX)

# Pre-Allocation特性仅支持Ascend 960PR/Ascend 960DT（dav-9201），样例固定该架构。
set(CMAKE_ASC_ARCHITECTURES "dav-9201" CACHE STRING "NPU architecture: dav-9201")

# 是否使能MTE2 Pre-Allocation：ON为使能样例，OFF为基线（不使能）。分别编译对比MTE2流水利用率。
option(ENABLE_PRE_ALLOCATION "Enable MTE2 Pre-Allocation (true: enable, false: baseline)" ON)

add_executable(demo
    data_copy_pre_allocation.asc
)

message(STATUS "[INFO] CMAKE_ASC_ARCHITECTURES is ${CMAKE_ASC_ARCHITECTURES}")
message(STATUS "[INFO] ENABLE_PRE_ALLOCATION is ${ENABLE_PRE_ALLOCATION}")

if(ENABLE_PRE_ALLOCATION)
    target_compile_definitions(demo PRIVATE ENABLE_PRE_ALLOCATION=1)
else()
    target_compile_definitions(demo PRIVATE ENABLE_PRE_ALLOCATION=0)
endif()

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
