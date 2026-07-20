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

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")

# Probe 1 is an opt-in rebuild used to实测Case 1（无SIMT VF）场景下静态UB可用上限，
# 默认关闭，不影响4个正向case的默认编译运行路径。
option(ENABLE_UB_PROBE1 "Build the Case1 no-SIMT-VF static UB limit probe kernel" OFF)
set(UB_PROBE1_STATIC_BYTES "253920" CACHE STRING "Probe1 static UB size in bytes (no-SIMT-VF experiment)")
option(UB_PROBE1_NO_VF "Probe1 kernel fills the static UB directly from MainScalar without calling any VF function" OFF)
option(UB_PROBE1_DISABLE_RESERVED "Pass --cce-disable-vf-stack-reserved-ubuf when building probe1 (NOTE: not supported by CANN 9.0.0 bisheng, kept for forward compatibility)" OFF)

add_executable(ub_usage_limit
    ub_usage_limit.asc
)

target_compile_options(ub_usage_limit PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)

# platform_ascendc::PlatformAscendCManager/PlatformAscendC 的实现来自 platform/tiling_api，
# 用于实测 GetCoreMemSize(CoreMemType::UB) 在无VF/有SIMD VF/有SIMT VF场景下的返回值。
target_link_libraries(ub_usage_limit PRIVATE
    m
    platform
    tiling_api
    dl
)

if(ENABLE_UB_PROBE1)
    target_compile_definitions(ub_usage_limit PRIVATE
        ENABLE_UB_PROBE1=1
        UB_PROBE1_STATIC_BYTES=${UB_PROBE1_STATIC_BYTES}
    )
    if(UB_PROBE1_NO_VF)
        target_compile_definitions(ub_usage_limit PRIVATE UB_PROBE1_NO_VF=1)
    endif()
    if(UB_PROBE1_DISABLE_RESERVED)
        target_compile_options(ub_usage_limit PRIVATE
            $<$<COMPILE_LANGUAGE:ASC>:--cce-disable-vf-stack-reserved-ubuf>
        )
    endif()
endif()
