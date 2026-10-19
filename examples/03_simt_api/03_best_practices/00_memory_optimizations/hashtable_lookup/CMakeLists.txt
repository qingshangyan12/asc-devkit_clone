# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software; you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You should not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU ARCH, e.g. dav-3510")
set(CMAKE_ASC_ENABLE_SIMT ON)

find_package(ASC REQUIRED)
project(hashtable_lookup LANGUAGES ASC CXX)

set(RUN_MODE "npu" CACHE STRING "Run mode: npu, sim")
set(SOC_VERSION "" CACHE STRING "Simulator SOC version, e.g. Ascend950PR_9599")

# 场景参数（通过命令行传入）
# SCENARIO_NUM: 场景编号（一 case 一访问方式，各是独立算子）
#   0 - aos：AoS 基线，40B 槽步长，thread-per-key
#   1 - soa：SoA 纯布局，key 区 8B 步长，thread-per-key
#   2 - soa digest + swar：1B digest 预过滤 + 4 槽 word/vcmp_eq4 批量比较
#   3 - coop + swar：G=4 协作 × 每 lane SWAR 4 槽（16 槽/轮窗口）
set(SCENARIO_NUM "3" CACHE STRING "Case type to run (0-3)")
if(NOT SCENARIO_NUM MATCHES "^[0-3]$")
    message(FATAL_ERROR "SCENARIO_NUM must be one of 0, 1, 2, 3, but got ${SCENARIO_NUM}")
endif()

add_executable(hashtable_lookup
    hashtable_lookup.asc
)

target_compile_definitions(hashtable_lookup PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
)

target_compile_options(hashtable_lookup PRIVATE
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:-g -O0>>
    $<BUILD_INTERFACE:$<$<NOT:$<STREQUAL:${RUN_MODE},sim>>:-O2>>
    --cce-res-usage
)

target_link_directories(hashtable_lookup PRIVATE
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:$ENV{ASCEND_HOME_PATH}/tools/simulator/${SOC_VERSION}/lib>>
)

target_link_libraries(hashtable_lookup PRIVATE
    m
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:runtime_camodel>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:npu_drv>>
)
