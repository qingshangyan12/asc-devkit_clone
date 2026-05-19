# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(test_arch_refactor LANGUAGES ASC CXX)

set(ASC_DEVKIT_INCLUDE /home/q00954432/asc-devkit/include)

# Pure SIMT: --enable-simt sets __NPU_COMPILER_INTERNAL_PURE_SIMT__
# Verifies: __NPU_HAS_FP16/BF16/WARP/VECTOR_TYPES/ATOMIC__ (fp8 excluded: known compiler limitation in pure SIMT)
add_executable(test_pure test_pure.asc)
target_include_directories(test_pure BEFORE PRIVATE ${ASC_DEVKIT_INCLUDE})
target_compile_options(test_pure PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510 --enable-simt>
)

# Mixed mode: no --enable-simt, SIMT kernel called from vector core via asc_vf_call
# Verifies: same as above + __NPU_HAS_FP8__ + __NPU_HAS_VF_CALL__ (asc_vf_call)
add_executable(test_mixed test_mixed.asc)
target_include_directories(test_mixed BEFORE PRIVATE ${ASC_DEVKIT_INCLUDE})
target_compile_options(test_mixed PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
