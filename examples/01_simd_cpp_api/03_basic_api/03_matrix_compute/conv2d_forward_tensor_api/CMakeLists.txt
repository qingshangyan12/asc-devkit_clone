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
set(CMAKE_ASC_RUN_MODE "sim" CACHE STRING "Run mode: npu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    conv2d_forward_tensor_api.asc
)

# This sample exercises Tensor API conv paths (img2col, NC1HWC0) that are newer than the CANN
# package's bundled tensor_api headers, so the local repo's include/impl trees must take priority.
# get_filename_component(ASC_DEVKIT_REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.." ABSOLUTE)
# target_include_directories(demo BEFORE PRIVATE
#     ${ASC_DEVKIT_REPO_ROOT}
#     ${ASC_DEVKIT_REPO_ROOT}/include
# )
set(ASCEND_HOME $ENV{ASCEND_HOME_PATH})

set(ASC_INCLUDE_DIR "${ASCEND_HOME}/asc/")
include_directories(
    "${ASC_INCLUDE_DIR}"
)

# 场景参数（通过命令行传入）
# SCENARIO_NUM: 输入/输出 GM 格式场景编号
#   0 - GM 输入输出均为 NC1HWC0
#   1 - GM 输入输出均为 NCHW
#   2 - GM 输入输出均为 NHWC
set(SCENARIO_NUM "0" CACHE STRING "Scenario number (0=NC1HWC0, 1=NCHW, 2=NHWC)")
add_definitions(-DSCENARIO_NUM=${SCENARIO_NUM})

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
