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

project(kernel_samples)

set(ASC_SRC_FILE muls.asc)

# ======================================================================================
# 运行模式配置
#
# 说明：
#   - RUN_MODE=cpu: CPU 调试模式，无需 NPU 硬件，支持 GDB 调试
#   - RUN_MODE=sim: 仿真模式（暂未实现）
#   - RUN_MODE=npu: NPU 模式（默认），需要 Ascend NPU 硬件
# ======================================================================================
if("${RUN_MODE}" STREQUAL "cpu")
    find_package(tikicpulib REQUIRED)
    set_source_files_properties(${ASC_SRC_FILE} PROPERTIES LANGUAGE CXX)
else()
    find_package(ASC REQUIRED HINTS $ENV{ASCEND_INSTALL_PATH}/compiler/tikcpp/ascendc_kernel_cmake)
    enable_language(ASC CXX)
endif()

add_executable(demo
    ${ASC_SRC_FILE}
)

# ======================================================================================
# 编译选项配置
# ======================================================================================
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
    # $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},cpu>:-x c++>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},cpu>:-g -O2 -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -Werror>>
)

# ======================================================================================
# 链接库配置
# ======================================================================================
target_link_libraries(demo PRIVATE
    m
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},cpu>:cpudebug_acl_stub>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},cpu>:tikicpulib::ascend950pr_9599>>
    $<BUILD_INTERFACE:$<$<NOT:$<STREQUAL:${RUN_MODE},cpu>>:tiling_api>>
    $<BUILD_INTERFACE:$<$<NOT:$<STREQUAL:${RUN_MODE},cpu>>:register>>
    $<BUILD_INTERFACE:$<$<NOT:$<STREQUAL:${RUN_MODE},cpu>>:platform>>
    $<BUILD_INTERFACE:$<$<NOT:$<STREQUAL:${RUN_MODE},cpu>>:dl>>
)
