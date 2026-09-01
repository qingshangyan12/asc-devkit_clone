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

set(PROJECT_NAME "kernel_samples" CACHE STRING "Project name")
project(PROJECT_NAME LANGUAGES ASC CXX)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu, sim")

# 场景参数（通过命令行传入）
# SCENARIO_NUM: 场景编号
#   1 - DIFF_COMPENSATION + --cce-ftz=false：差值补偿算法，保留Subnormal，最大精度误差为0ulp
#   2 - DIFF_COMPENSATION + --cce-ftz=true：差值补偿算法，FTZ模式，最大精度误差为0ulp
set(SCENARIO_NUM "1" CACHE STRING "Scene number (1-2)")
if(NOT SCENARIO_NUM MATCHES "^[1-2]$")
    message(FATAL_ERROR "SCENARIO_NUM must be 1 or 2, but got ${SCENARIO_NUM}")
endif()
add_definitions(-DSCENARIO_NUM=${SCENARIO_NUM})

set(TARGET_FILENAME "demo" CACHE STRING "Execution file name")
add_executable(${TARGET_FILENAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/subnormal.asc
)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")
if(SCENARIO_NUM EQUAL 1)
    set(CCE_FTZ_OPTION "--cce-ftz=false")
else()
    set(CCE_FTZ_OPTION "--cce-ftz=true")
endif()
target_compile_options(${TARGET_FILENAME} PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
        $<$<COMPILE_LANGUAGE:ASC>:${CCE_FTZ_OPTION}>
)
