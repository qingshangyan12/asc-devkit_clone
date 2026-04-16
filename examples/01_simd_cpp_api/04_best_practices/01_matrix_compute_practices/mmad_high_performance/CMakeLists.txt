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

if(DEFINED RUN_MODE)
    set(CMAKE_ASC_RUN_MODE ${RUN_MODE} CACHE STRING "Alias of RUN_MODE")
endif()

if(DEFINED NPU_ARCH)
    set(CMAKE_ASC_ARCHITECTURES ${NPU_ARCH} CACHE STRING "Alias of NPU_ARCH")
endif()

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    mmad.asc
)

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    m
    dl
)
set(NPU_ARCH "dav-2201" CACHE STRING "NPU ARCH, e.g. dav-2201, dav-3510")

# ASC 编译器通过 --npu-arch 自动为 device 侧（__aicore__ 代码）定义 __NPU_ARCH__，
# 但 host 侧（main 函数等 C++ 代码）无法看到此内置宏。
# 因此显式定义 __ASC_NPU_ARCH__ 供整个编译单元（host + device）共用，
# 使 main() 中的架构分支也能正确工作。
if(NPU_ARCH STREQUAL "dav-2201")
    set(_NPU_ARCH_NUM 2201)
elseif(NPU_ARCH STREQUAL "dav-3510")
    set(_NPU_ARCH_NUM 3510)
else()
    message(FATAL_ERROR "Unsupported NPU_ARCH: ${NPU_ARCH}, must be dav-2201 or dav-3510")
endif()

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${NPU_ARCH} -D__ASC_NPU_ARCH__=${_NPU_ARCH_NUM}>
)