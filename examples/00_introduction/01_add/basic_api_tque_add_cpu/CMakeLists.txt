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

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(add
    add.asc
)

# ======================================================================================
# NPU 编译选项配置
#
# 说明：
#   - 需根据实际部署的 NPU 硬件架构选择对应的 `npu-arch` 参数。
# ======================================================================================
target_compile_options(add PRIVATE
    # $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201 ; --run-mode=cpu>
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
    $<$<COMPILE_LANGUAGE:ASC>:--run-mode=cpu>
    -v
    -std=c++17
    -D__CCE_KT_TEST__=1
    -DASCENDC_DEBUG=1
    -D_GLIBCXX_USE_CXX11_ABI=0
    -D__NPU_ARCH__=2201
)

target_include_directories(add PRIVATE
    $ENV{ASCEND_HOME_PATH}/tools/tikicpulib/lib/include
)

target_link_options(add PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>: --run-mode=cpu>
    -L$ENV{ASCEND_HOME_PATH}/tools/tikicpulib/lib
    -L$ENV{ASCEND_HOME_PATH}/tools/tikicpulib/lib/Ascend910B1
    -L$ENV{ASCEND_HOME_PATH}/tools/simulator/Ascend910B1/lib
    -L$ENV{ASCEND_HOME_PATH}/lib64
    -Wl,rpath,$ENV{ASCEND_HOME_PATH}/tools/tikicpulib/lib
    -Wl,rpath,$ENV{ASCEND_HOME_PATH}/tools/tikicpulib/lib/Ascend910B1
    -Wl,rpath,$ENV{ASCEND_HOME_PATH}/tools/simulator/Ascend910B1/lib
    -Wl,rpath,$ENV{ASCEND_HOME_PATH}/lib64
)

target_link_libraries(add PRIVATE
    -Wl,-Bdynamic,--no-as-needed
    m
    ascendc_acl_stub
    cpudebug_cceprint
    cpudebug_npuchk
    cpudebug_stubreg
    pem_davinci
    cpudebug
    c_sec
    stdc++
    -Wl,--as-needed
)

