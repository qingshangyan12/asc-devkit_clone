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

set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")

find_package(ASC REQUIRED)

project(cache_consistency_verify_c_api LANGUAGES ASC CXX)

add_executable(demo_c_api
    cache_consistency_verify_c_api.asc
)

add_executable(cacheline_overwrite_demo_c_api
    multi_core_cacheline_overwrite_c_api.asc
)

add_executable(scalar_gm_211_demo_c_api
    scalar_gm_211_demo_c_api.asc
)

add_executable(ub_dcache_241_demo_c_api
    ub_dcache_241_demo_c_api.asc
)

add_executable(memory_consistency_demo_c_api
    memory_consistency_verify_c_api.asc
)

foreach(target demo_c_api cacheline_overwrite_demo_c_api scalar_gm_211_demo_c_api ub_dcache_241_demo_c_api memory_consistency_demo_c_api)
    target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    )
endforeach()
