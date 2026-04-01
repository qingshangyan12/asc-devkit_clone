# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software; you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

# 场景配置（通过编译时宏传递）
# SCENARIO_NUM: 场景编号
#   1 - Sum：每行求和，输入shape [3, 32]，输出shape [8]（对齐后）
#   2 - Mean：每行求均值，输入shape [3, 32]，输出shape [8]（对齐后）
set(SCENARIO_NUM "1" CACHE STRING "场景编号 (1=Sum求和, 2=Mean求均值)")

add_definitions(-DSCENARIO_NUM=${SCENARIO_NUM})

add_executable(demo
    sum-mean.asc
)

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    m
    dl
)

# ======================================================================================
# NPU 架构选择配置
#
# 说明：
#   - 根据实际部署的 NPU 硬件架构选择对应的 `npu-arch` 参数值
# ======================================================================================
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
    # $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
