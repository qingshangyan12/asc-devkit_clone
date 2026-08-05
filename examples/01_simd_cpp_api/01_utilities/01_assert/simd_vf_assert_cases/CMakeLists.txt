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

project(simd_vf_assert_cases LANGUAGES ASC CXX)

foreach(case_id RANGE 0 18)
    set(target_name assert_case_${case_id})
    add_executable(${target_name}
        simd_vf_assert_cases.asc
    )
    target_compile_definitions(${target_name} PRIVATE
        ASSERT_CASE_ID=${case_id}
    )
    if(case_id EQUAL 13)
        target_compile_definitions(${target_name} PRIVATE ASCENDC_DUMP=0)
    elseif(case_id EQUAL 14)
        target_compile_definitions(${target_name} PRIVATE NDEBUG)
    endif()
    target_compile_options(${target_name} PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES};--cce-long-scbz=true>
    )
    target_link_libraries(${target_name} PRIVATE
        acl_rt
        ascendcl
    )
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endforeach()
