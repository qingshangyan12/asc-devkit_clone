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

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

find_package(ASC REQUIRED)

project(hccl_link_select LANGUAGES ASC CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED ASCEND_HOME_PATH)
    if(DEFINED ENV{ASCEND_HOME_PATH})
        set(ASCEND_HOME_PATH $ENV{ASCEND_HOME_PATH})
    else()
        set(ASCEND_HOME_PATH /usr/local/Ascend/ascend-toolkit/latest)
    endif()
endif()

add_executable(hccl_link_select
    main.cpp
    ubmem_data_copy.asc
)

set_target_properties(hccl_link_select PROPERTIES
    LINKER_LANGUAGE ASC
)

target_include_directories(hccl_link_select PRIVATE
    ${ASCEND_HOME_PATH}/include
)

target_link_directories(hccl_link_select PRIVATE
    ${ASCEND_HOME_PATH}/lib64
    ${ASCEND_HOME_PATH}/runtime/lib64
)

target_link_libraries(hccl_link_select PRIVATE hccl hcomm ascendcl c_sec pthread)

target_compile_options(hccl_link_select PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)

add_executable(hccl_link_select_debug EXCLUDE_FROM_ALL
    debug.asc
)

target_include_directories(hccl_link_select_debug PRIVATE
    ${ASCEND_HOME_PATH}/include
)

target_link_directories(hccl_link_select_debug PRIVATE
    ${ASCEND_HOME_PATH}/lib64
    ${ASCEND_HOME_PATH}/runtime/lib64
)

target_link_libraries(hccl_link_select_debug PRIVATE hccl hcomm ascendcl c_sec pthread)

target_compile_options(hccl_link_select_debug PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
