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

set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201")

find_package(ASC REQUIRED)

project(hcomm_exception_fault_cases LANGUAGES ASC CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(hcomm_exception_fault_kernels SHARED
    hcomm_exception_fault_cases.asc
)
set_target_properties(hcomm_exception_fault_kernels PROPERTIES
    LINKER_LANGUAGE ASC
)
target_include_directories(hcomm_exception_fault_kernels PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/basic_api
    $ENV{ASCEND_HOME_PATH}/include
)
target_compile_options(hcomm_exception_fault_kernels PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
add_library(hcomm_channel_dump SHARED
    hcomm_channel_dump_shim.cpp
)
target_compile_definitions(hcomm_channel_dump PRIVATE
    _GLIBCXX_USE_CXX11_ABI=0
)
target_link_libraries(hcomm_channel_dump PRIVATE
    dl
)

add_executable(exception_callback_demo
    exception_callback_demo.cpp
)
target_compile_definitions(exception_callback_demo PRIVATE
    _GLIBCXX_USE_CXX11_ABI=0
)
target_include_directories(exception_callback_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    $ENV{ASCEND_HOME_PATH}/include
)
target_link_directories(exception_callback_demo PRIVATE
    $ENV{ASCEND_HOME_PATH}/lib64
)
target_link_libraries(exception_callback_demo PRIVATE
    dl
    acl_rt
)

add_executable(hcomm_exception_fault_runner
    hcomm_exception_fault_runner.cpp
)
target_compile_definitions(hcomm_exception_fault_runner PRIVATE
    _GLIBCXX_USE_CXX11_ABI=0
)
target_include_directories(hcomm_exception_fault_runner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    $ENV{ASCEND_HOME_PATH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/adv_api/hccl/internal/hcomm
)
target_link_directories(hcomm_exception_fault_runner PRIVATE
    $ENV{ASCEND_HOME_PATH}/lib64
    /usr/local/Ascend/driver/lib64/driver
    $ENV{ASCEND_HOME_PATH}/devlib
)
target_link_libraries(hcomm_exception_fault_runner PRIVATE
    hcomm_exception_fault_kernels
    dl
    acl_rt
    hccl
    hcomm
    c_sec
    ascend_hal
)
