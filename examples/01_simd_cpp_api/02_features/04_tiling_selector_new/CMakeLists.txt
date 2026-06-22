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

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")
set(SCENARIO_NUM "0" CACHE STRING "Tiling scenario number: 0-3")

if(NOT SCENARIO_NUM MATCHES "^[0-3]$")
    message(FATAL_ERROR "SCENARIO_NUM must be 0, 1, 2 or 3, but got ${SCENARIO_NUM}")
endif()

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

message(STATUS "[INFO] SCENARIO_NUM is ${SCENARIO_NUM}")

add_executable(demo
    add.asc
)

add_executable(demo_unified
    add_unified_tiling.asc
)

foreach(target demo demo_unified)
    target_compile_definitions(${target} PRIVATE
        SCENARIO_NUM=${SCENARIO_NUM}
    )

    target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    )
endforeach()
