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

# 场景参数（通过命令行传入）
# SCENARIO_NUM: 场景编号
#   1 - TBufPool内存复用
#   2 - TBufPool资源细分
#   3 - 自定义TBufPool
set(SCENARIO_NUM "" CACHE STRING "Scene number (1-3)")

add_definitions(-DSCENARIO_NUM=${SCENARIO_NUM})

set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    tbufpool_management.asc
)

message(STATUS "[INFO] SCENARIO_NUM is ${SCENARIO_NUM}")

# 校验 SCENARIO_NUM 是否定义
if(NOT DEFINED SCENARIO_NUM OR SCENARIO_NUM STREQUAL "")
    message(FATAL_ERROR "SCENARIO_NUM is not set. Please specify -DSCENARIO_NUM=1/2/3")
endif()

# 校验 SCENARIO_NUM 范围
set(VALID_SCENARIOS 1 2 3)
if(NOT SCENARIO_NUM IN_LIST VALID_SCENARIOS)
    message(FATAL_ERROR "SCENARIO_NUM must be 1,2,3, got: ${SCENARIO_NUM}")
endif()

target_compile_definitions(demo PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
