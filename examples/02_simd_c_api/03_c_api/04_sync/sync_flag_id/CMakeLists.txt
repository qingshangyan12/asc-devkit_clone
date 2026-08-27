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

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu")
set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201 or dav-3510")

find_package(ASC REQUIRED)

project(c_api_sync_flag_id LANGUAGES ASC CXX)

if(NOT CMAKE_ASC_ARCHITECTURES MATCHES "^(dav-2201|dav-3510)$")
    message(FATAL_ERROR "CMAKE_ASC_ARCHITECTURES must be dav-2201 or dav-3510.")
endif()

if(NOT CMAKE_ASC_RUN_MODE STREQUAL "npu")
    message(FATAL_ERROR "This example only supports CMAKE_ASC_RUN_MODE=npu.")
endif()

add_executable(demo sync_flag_id.asc)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
