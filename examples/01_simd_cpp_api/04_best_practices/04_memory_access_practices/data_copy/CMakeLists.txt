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

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")
set(SCENARIO_NUM "1" CACHE STRING "Scenario number: UB supports 1-6, L1 supports 1-4")
set(COPY_DST "UB" CACHE STRING "Copy destination: UB, L1")

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

if(NOT COPY_DST STREQUAL "UB" AND NOT COPY_DST STREQUAL "L1")
    message(FATAL_ERROR "COPY_DST must be UB or L1, but got ${COPY_DST}")
endif()

if(COPY_DST STREQUAL "UB")
    if(NOT SCENARIO_NUM MATCHES "^[1-6]$")
        message(FATAL_ERROR "COPY_DST=UB supports SCENARIO_NUM 1, 2, 3, 4, 5 or 6, but got ${SCENARIO_NUM}")
    endif()
elseif(COPY_DST STREQUAL "L1")
    if(NOT SCENARIO_NUM MATCHES "^[1-4]$")
        message(FATAL_ERROR "COPY_DST=L1 supports SCENARIO_NUM 1, 2, 3 or 4, but got ${SCENARIO_NUM}")
    endif()
endif()

add_executable(demo
    data_copy.asc
)

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
)

message(STATUS "[INFO] SCENARIO_NUM is ${SCENARIO_NUM}")
message(STATUS "[INFO] COPY_DST is ${COPY_DST}")

if(COPY_DST STREQUAL "UB")
    target_compile_definitions(demo PRIVATE COPY_DST_UB=1)
elseif(COPY_DST STREQUAL "L1")
    target_compile_definitions(demo PRIVATE COPY_DST_L1=1)
endif()

target_compile_definitions(demo PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
