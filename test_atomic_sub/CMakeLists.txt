# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(test_atomic_sub LANGUAGES ASC CXX)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU ARCH, e.g. dav-3510")

add_executable(test_atomic_sub
    test_atomic_sub.asc
)

target_compile_options(test_atomic_sub PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)

target_link_libraries(test_atomic_sub PRIVATE
    m
)

# 边界值测试 —— 混编形态（__simt_vf__ + asc_vf_call + 3-slot launch，不带 --enable-simt）
add_executable(test_atomic_sub_boundary_mixed
    test_atomic_sub_boundary_mixed.asc
)

target_compile_options(test_atomic_sub_boundary_mixed PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)

target_link_libraries(test_atomic_sub_boundary_mixed PRIVATE
    m
)

# 边界值测试 —— pure SIMT 形态（__global__ void + 4-slot launch + --enable-simt）
add_executable(test_atomic_sub_boundary_pure
    test_atomic_sub_boundary_pure.asc
)

target_compile_options(test_atomic_sub_boundary_pure PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES} --enable-simt>
)

target_link_libraries(test_atomic_sub_boundary_pure PRIVATE
    m
)
