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

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

find_package(ASC REQUIRED)

project(c_api_asc_copy_gm2l1 LANGUAGES ASC CXX)

if(NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    message(FATAL_ERROR "This C API sample only supports CMAKE_ASC_ARCHITECTURES=dav-3510.")
endif()

set(SCENARIO_NUM "1" CACHE STRING "Scene number: 1 (NZ), 2 (ND), 3 (DN), 4 (Bias), or 5 (vector quantization)")
if(NOT SCENARIO_NUM MATCHES "^[1-5]$")
    message(FATAL_ERROR "SCENARIO_NUM must be 1, 2, 3, 4, or 5, but got ${SCENARIO_NUM}.")
endif()

add_executable(demo asc_copy_gm2l1.asc)

target_compile_definitions(demo PRIVATE SCENARIO_NUM=${SCENARIO_NUM})
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
