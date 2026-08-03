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

project(matmul_c_api_sample LANGUAGES ASC CXX)

add_executable(demo matmul.asc)

# Keep the C API headers from this asc-devkit checkout ahead of the toolkit's
# compatibility headers.  This is required for the dav-3510 Cube C API, whose
# declarations are shipped with asc-devkit and may be newer than the toolkit
# wrapper package installed on a development machine.
get_filename_component(ASC_DEVKIT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../" ABSOLUTE)
target_include_directories(demo PRIVATE
    "${ASC_DEVKIT_ROOT}/include"
    "${ASC_DEVKIT_ROOT}"
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
