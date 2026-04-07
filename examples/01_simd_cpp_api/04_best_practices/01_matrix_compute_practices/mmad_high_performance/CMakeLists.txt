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
    mmad.asc
)

set(RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu or sim")
set(NPU_ARCH "dav-2201" CACHE STRING "NPU architecture, e.g. dav-2201, dav-3510")

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    m
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
        unified_dlog
        -pthread
        stdc++
        -Wl,--as-needed
    )
elseif(RUN_MODE STREQUAL "sim")
    # ====================simulator模式====================
    set(INSTALL_DIR "$ENV{ASCEND_HOME_PATH}" CACHE PATH "CANN install directory")
    string(REPLACE "-" "_" SIM_ARCH "${NPU_ARCH}")

    set_target_properties(demo PROPERTIES
        LINK_FLAGS "-Wl,--disable-new-dtags"
    )

    target_link_libraries(demo PRIVATE
        runtime_camodel npu_drv_camodel
        m
    )

    target_link_directories(demo PRIVATE
        ${INSTALL_DIR}/tools/simulator/${SIM_ARCH}/lib
    )
endif()