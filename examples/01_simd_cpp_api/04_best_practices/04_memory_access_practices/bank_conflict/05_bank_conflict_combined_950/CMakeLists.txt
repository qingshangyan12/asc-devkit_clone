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

set(SCENARIO "1" CACHE STRING "Combined bank conflict scenario (1=both conflict, 2=MTE2 fixed, 3=both fixed)")
if (NOT SCENARIO MATCHES "^[123]$")
    message(FATAL_ERROR "SCENARIO must be 1, 2, or 3")
endif()

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    bank_conflict_combined_950.asc
)

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    m
    dl
)

target_compile_definitions(demo PRIVATE
    SCENARIO=${SCENARIO}
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
