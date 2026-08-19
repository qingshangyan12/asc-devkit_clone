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

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")

find_package(ASC REQUIRED)

project(cce_builtin_vcvt_compare LANGUAGES ASC CXX)

if(NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    message(FATAL_ERROR "This example only supports CMAKE_ASC_ARCHITECTURES=dav-3510")
endif()

function(add_vcvt_compare_target target_name use_builtin)
    add_executable(${target_name}
        cce_builtin_vcvt_compare.asc
    )
    target_compile_definitions(${target_name} PRIVATE
        USE_VCVT_BUILTIN=${use_builtin}
    )
    target_compile_options(${target_name} PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES};-O2;-Wno-deprecated-declarations>
    )
endfunction()

add_vcvt_compare_target(cce_vcvt_compare 0)
add_vcvt_compare_target(builtin_vcvt_compare 1)
