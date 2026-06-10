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

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-2201, dav-3510")
set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(SCENARIO_NUM "1" CACHE STRING "Scene: 1=Nd2Nz, 2=repeat, 3=显式SetFmatrix/PaddingValue (均dav-3510/dav-2201), 4=Dn2Nz(dav-3510 only)")

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

if(NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510" AND NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-2201")
    message(FATAL_ERROR "conv2d_forward supports dav-3510 and dav-2201, but got ${CMAKE_ASC_ARCHITECTURES}")
endif()

if(NOT SCENARIO_NUM MATCHES "^[1234]$")
    message(FATAL_ERROR "SCENARIO_NUM must be 1, 2, 3 or 4, but got ${SCENARIO_NUM}")
endif()

if(SCENARIO_NUM STREQUAL "4" AND NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    message(FATAL_ERROR "SCENARIO_NUM=4 (Dn2Nz path) is only supported on dav-3510, but got ${CMAKE_ASC_ARCHITECTURES}")
endif()

message(STATUS "[INFO] SCENARIO_NUM is ${SCENARIO_NUM}")

add_executable(demo
    conv2d_forward.asc
)

target_compile_definitions(demo PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
