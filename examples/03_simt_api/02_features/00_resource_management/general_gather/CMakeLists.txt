# ----------------------------------------------------------------------------------------------------------
# This program is free software, you can redistribute it and/or modify it.
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This file is a part of the CANN Open Software.
# Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

project(kernel_samples)

find_package(ASC REQUIRED)
enable_language(ASC CXX)

function(add_asc_executable TARGET_NAME SOURCE_FILE)
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
    
    # ======================================================================================
    # NPU 编译选项配置
    #
    # 说明：
    #   - 需根据实际部署的 NPU 硬件架构选择对应的 `npu-arch` 参数。
    # ======================================================================================
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510 --enable-simt -O3 -std=c++17>
        $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:-g -O0>>
    )
    
    target_link_directories(${TARGET_NAME} PRIVATE
        $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:$ENV{ASCEND_HOME_PATH}/tools/simulator/${SOC_VERSION}/lib>>
    )
    
    target_link_libraries(${TARGET_NAME} PRIVATE 
        m
        $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:runtime_camodel>>
        $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:npu_drv>>
        platform
        tiling_api
        dl
    )
endfunction()

# ======================================================================================
# 创建可执行文件
# ======================================================================================
add_asc_executable(gather gather.asc)
add_asc_executable(gather_v2 gather_v2.asc)