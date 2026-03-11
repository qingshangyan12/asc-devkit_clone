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

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

# 允许用户在配置时选择源文件（默认 mmad_fixpipe_after_all_mmad.asc）
set(DEMO_SOURCE "mmad_fixpipe_after_all_mmad.asc" 
    CACHE STRING "Choose the source file for demo (mmad_fixpipe_after_all_mmad.asc / mmad_fixpipe_per_mmad.asc / mmad_unitflag.asc)"
)

# 检查所选文件是否存在
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${DEMO_SOURCE})
    message(FATAL_ERROR "Source file ${DEMO_SOURCE} not found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

add_executable(demo ${DEMO_SOURCE})

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    m
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
)
