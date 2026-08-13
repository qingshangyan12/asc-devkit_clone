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

set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    fixpipe_l0c2gm.asc
)

target_link_libraries(demo PRIVATE)

# 场景参数（通过命令行传入）
# SCENARIO_NUM: 场景编号
#   1-7:  原有场景（float L0C, half输入），支持 dav-2201 和 dav-3510（场景3仅dav-3510）
#   8-41: float L0C 新数据类型组合（仅 dav-3510）
#   42-59: int32_t L0C 数据类型组合（仅 dav-3510）
set(SCENARIO_NUM "1" CACHE STRING "Scene number (1-64)")
if(SCENARIO_NUM LESS 1 OR SCENARIO_NUM GREATER 64)
    message(FATAL_ERROR "SCENARIO_NUM must be 1-64, but got ${SCENARIO_NUM}")
endif()

# 场景3及场景8-64 仅支持 dav-3510
if((SCENARIO_NUM STREQUAL "3" OR SCENARIO_NUM GREATER_EQUAL "8") AND CMAKE_ASC_ARCHITECTURES STREQUAL "dav-2201")
    message(FATAL_ERROR "SCENARIO_NUM=${SCENARIO_NUM} only supports dav-3510, not dav-2201")
endif()

add_definitions(-DSCENARIO_NUM=${SCENARIO_NUM})

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
