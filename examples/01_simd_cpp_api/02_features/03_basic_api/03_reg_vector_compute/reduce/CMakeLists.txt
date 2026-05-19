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

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

# 定义所有需要编译的 .asc 文件及其对应的可执行目标名
set(ASC_SOURCES
    reduce_accumulate.asc
    reduce_iterator.asc
    reduce_binary.asc
    reduce_max.asc
    reduce_min.asc
)

# 批量创建可执行目标
foreach(source ${ASC_SOURCES})
    get_filename_component(target_name ${source} NAME_WE)
    set(target "demo_${target_name}")
    
    add_executable(${target} ${source})
    
    target_link_libraries(${target} PRIVATE
        tiling_api
        register
        platform
        m
    )
    
    target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
        $<$<COMPILE_LANGUAGE:ASC>:--cce-simd-vf-fusion=false>
    )
endforeach()
