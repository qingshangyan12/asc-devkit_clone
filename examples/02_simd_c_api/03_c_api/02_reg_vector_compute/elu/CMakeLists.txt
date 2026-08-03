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

# ASC 语言的 CMake 模块（CMakeDetermineASCCompiler / CMakeASCInformation 等）由
# find_package(ASC) 动态加载，而 project() 启用 ASC 语言时即需要这些模块。
# 故在 project() 之前显式将 asc_modules 加入 CMAKE_MODULE_PATH，保证常规的
# cmake_minimum_required → project → find_package(ASC) 顺序下工具链变量就绪。
if(NOT DEFINED ENV{ASCEND_HOME_PATH})
    message(FATAL_ERROR "ASCEND_HOME_PATH is not set. Please source the CANN set_env.sh first.")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{ASCEND_HOME_PATH}/compiler/tikcpp/ascendc_kernel_cmake/asc_modules")

project(kernel_samples LANGUAGES ASC CXX)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

find_package(ASC REQUIRED)

add_executable(demo
    elu.asc
)

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    m  # Host侧 std::exp/std::expm1（golden计算）需链接系统libm（ASC链接器默认不链）
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
