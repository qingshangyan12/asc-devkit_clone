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

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510 for Ascend 950PR/950DT")

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

option(SCENARIO_NUM "1" "场景编号：1=GetUBSizeInBytes, 2=GetRuntimeUBSize")
set(VALID_SCENARIOS 1 2)
if(NOT SCENARIO_NUM IN_LIST VALID_SCENARIOS)
    message(FATAL_ERROR "SCENARIO_NUM must be 1 or 2")
endif()

add_executable(demo
    get_ub_size.asc
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    $<$<COMPILE_LANGUAGE:ASC>:-DSCENARIO_NUM=${SCENARIO_NUM}>
)