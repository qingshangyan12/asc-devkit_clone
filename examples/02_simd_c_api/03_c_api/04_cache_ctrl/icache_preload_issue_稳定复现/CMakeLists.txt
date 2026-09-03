# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

find_package(ASC REQUIRED)

project(icache_preload_issue LANGUAGES ASC CXX)

if(NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    message(FATAL_ERROR "This reproducer only supports CMAKE_ASC_ARCHITECTURES=dav-3510.")
endif()

add_library(icache_probe_kernel SHARED
    icache_probe_kernel.asc
)

add_library(unrelated_probe_kernel SHARED
    unrelated_probe_kernel.asc
)

set_target_properties(icache_probe_kernel unrelated_probe_kernel PROPERTIES
    LINKER_LANGUAGE ASC
)

add_executable(demo
    main.cpp
)

set_target_properties(demo PROPERTIES
    LINKER_LANGUAGE ASC
)

add_dependencies(demo
    icache_probe_kernel
    unrelated_probe_kernel
)

target_compile_options(icache_probe_kernel PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES} -fPIC>
)

target_compile_options(unrelated_probe_kernel PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES} -fPIC>
)

target_include_directories(demo PRIVATE
    $ENV{ASCEND_HOME_PATH}/include
)

target_link_directories(demo PRIVATE
    $ENV{ASCEND_HOME_PATH}/lib64
)

target_link_libraries(demo PRIVATE
    ascendcl
    dl
)
