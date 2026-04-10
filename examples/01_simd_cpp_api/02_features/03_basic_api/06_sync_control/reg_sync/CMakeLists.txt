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

add_executable(demo
    reg_sync.asc
)

# ======================================================================================
# NPU 编译选项配置
#
# 说明：
#   - 需根据实际部署的 NPU 硬件架构选择对应的 `npu-arch` 参数。
# ======================================================================================
set(NPU_ARCH "dav-3510" CACHE STRING "NPU ARCH, e.g. dav-3510")

message(STATUS "[INFO] SCENARIO_NUM is ${SCENARIO_NUM}")

if(NOT DEFINED SCENARIO_NUM)
    message(FATAL_ERROR "SCENARIO_NUM is not set. Please specify -DSCENARIO_NUM=1 or -DSCENARIO_NUM=2")
endif()

if(NOT SCENARIO_NUM EQUAL 1 AND NOT SCENARIO_NUM EQUAL 2)
    message(FATAL_ERROR "SCENARIO_NUM must be 1 or 2, got: ${SCENARIO_NUM}")
endif()

target_compile_definitions(demo PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${NPU_ARCH}>
    $<$<COMPILE_LANGUAGE:ASC>:--cce-simd-vf-fusion=false>
)
