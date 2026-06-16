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

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")
set(SCENARIO_NUM "1" CACHE STRING "Scenario number: 1-7")

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

if(NOT SCENARIO_NUM MATCHES "^[1-7]$")
    message(FATAL_ERROR "SCENARIO_NUM must be 1 to 7, but got ${SCENARIO_NUM}")
endif()

# 确定硬件架构 ID，用于 kernel 内编译期宏
if(CMAKE_ASC_ARCHITECTURES STREQUAL "dav-2201")
    set(NPU_ARCH_ID 2201)
elseif(CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    set(NPU_ARCH_ID 3510)
else()
    message(FATAL_ERROR "Unsupported CMAKE_ASC_ARCHITECTURES=${CMAKE_ASC_ARCHITECTURES}, only dav-2201 and dav-3510 are supported")
endif()

add_executable(demo
    l2_cache.asc
)

target_compile_definitions(demo PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
    NPU_ARCH_ID=${NPU_ARCH_ID}
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
