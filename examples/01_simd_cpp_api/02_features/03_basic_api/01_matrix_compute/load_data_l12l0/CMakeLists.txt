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

add_executable(demo
    load_data_l12l0.asc
)

# ======================================================================================
# 编译模式与 NPU 架构配置
#
# 说明：
#   - RUN_MODE: 编译运行模式，支持 npu（默认）和 cpu（CPU 调试模式）
#   - NPU_ARCH: NPU 硬件架构，例如 dav-2201、dav-3510（CPU 模式下 SOC_VERSION 由此自动推导）
#   - INSTALL_DIR: CANN 安装路径（cpu 模式下需要，默认读取 $ASCEND_HOME_PATH）
#
# 用法示例：
#   cmake -DRUN_MODE=npu -DNPU_ARCH=dav-2201 ..
#   cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..
#   cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-3510 ..
# ======================================================================================
set(RUN_MODE "npu" CACHE STRING "Run mode: npu or cpu")
set(NPU_ARCH "dav-2201" CACHE STRING "NPU architecture, e.g. dav-2201, dav-3510")

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    m
)
message(STATUS "[INFO] M_SIZE is ${M_SIZE}")
message(STATUS "[INFO] K_SIZE is ${K_SIZE}")
message(STATUS "[INFO] N_SIZE is ${N_SIZE}")
message(STATUS "[INFO] SCENARIO_NUM is ${SCENARIO_NUM}")
target_compile_definitions(demo PRIVATE
    M_SIZE=${M_SIZE}
    K_SIZE=${K_SIZE}
    N_SIZE=${N_SIZE}
    SCENARIO_NUM=${SCENARIO_NUM}
)

# ======================================================================================
# NPU 编译选项配置
#
# 说明：
#   - 需根据实际部署的 NPU 硬件架构选择对应的 `npu-arch` 参数。
# ======================================================================================
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${NPU_ARCH}>
)

if(RUN_MODE STREQUAL "cpu")
    # ====================cpu debug模式====================
    set(INSTALL_DIR "$ENV{ASCEND_HOME_PATH}" CACHE PATH "CANN install directory")

    if(NPU_ARCH STREQUAL "dav-2201")
        set(SOC_VERSION "Ascend910B1")
    elseif(NPU_ARCH STREQUAL "dav-3510")
        set(SOC_VERSION "Ascend950PR_9599")
    else()
        message(FATAL_ERROR "Unsupported NPU_ARCH for cpu mode: ${NPU_ARCH}")
    endif()

    target_compile_options(demo PRIVATE
        -g
        $<$<COMPILE_LANGUAGE:ASC>:--run-mode=cpu>
        -std=c++17
    )

    target_compile_definitions(demo PRIVATE
        _GLIBCXX_USE_CXX11_ABI=0
        ASCENDC_DEBUG=1
    )

    target_include_directories(demo PRIVATE
        ${INSTALL_DIR}/tools/tikicpulib/lib/include
    )

    target_link_options(demo PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--run-mode=cpu>
        -L${INSTALL_DIR}/tools/tikicpulib/lib
        -L${INSTALL_DIR}/tools/tikicpulib/lib/${SOC_VERSION}
        -L${INSTALL_DIR}/tools/simulator/${SOC_VERSION}/lib
        -L${INSTALL_DIR}/lib64
        -Wl,-rpath,${INSTALL_DIR}/tools/tikicpulib/lib
        -Wl,-rpath,${INSTALL_DIR}/tools/tikicpulib/lib/${SOC_VERSION}
        -Wl,-rpath,${INSTALL_DIR}/tools/simulator/${SOC_VERSION}/lib
        -Wl,-rpath,${INSTALL_DIR}/lib64
    )

    target_link_libraries(demo PRIVATE
        -Wl,-Bdynamic,--no-as-needed
        ascendc_acl_stub
        cpudebug_cceprint
        cpudebug_npuchk
        cpudebug_stubreg
        pem_davinci
        cpudebug
        c_sec
        -pthread
        stdc++
        -Wl,--as-needed
    )
endif()