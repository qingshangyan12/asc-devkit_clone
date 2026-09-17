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

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu or sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")
set(SCENARIO_NUM "1" CACHE STRING "Bank conflict scenario (1/2=Reg VF, 3/4=column-copy)")

if(NOT CMAKE_ASC_RUN_MODE STREQUAL "npu" AND NOT CMAKE_ASC_RUN_MODE STREQUAL "sim")
    message(FATAL_ERROR "CMAKE_ASC_RUN_MODE must be 'npu' or 'sim'")
endif()
if(NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510")
    message(FATAL_ERROR "bank_conflict only supports dav-3510")
endif()
if(NOT SCENARIO_NUM MATCHES "^[1234]$")
    message(FATAL_ERROR "SCENARIO_NUM must be 1, 2, 3 or 4")
endif()
# The full sample follows the design document (8192x8192 and 8x8 blocks).
# Simulator dimensions are cache variables so a small functional run can be
# selected without changing the production defaults.
if(CMAKE_ASC_RUN_MODE STREQUAL "sim")
    set(SAMPLE_TOTAL_M 4608 CACHE STRING "Simulator matrix M")
    set(SAMPLE_TOTAL_N 1024 CACHE STRING "Simulator matrix N")
    set(SAMPLE_ROW_SPLITS 2 CACHE STRING "Simulator row split count")
    set(SAMPLE_COL_SPLITS 2 CACHE STRING "Simulator column split count")
else()
    set(SAMPLE_TOTAL_M 8192 CACHE STRING "NPU matrix M")
    set(SAMPLE_TOTAL_N 8192 CACHE STRING "NPU matrix N")
    set(SAMPLE_ROW_SPLITS 8 CACHE STRING "NPU row split count")
    set(SAMPLE_COL_SPLITS 8 CACHE STRING "NPU column split count")
endif()

find_package(ASC REQUIRED)
project(bank_conflict LANGUAGES ASC CXX)

add_executable(demo bank_conflict.asc)
target_compile_definitions(demo PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
    BANK_CONFLICT_TOTAL_M=${SAMPLE_TOTAL_M}
    BANK_CONFLICT_TOTAL_N=${SAMPLE_TOTAL_N}
    BANK_CONFLICT_ROW_SPLITS=${SAMPLE_ROW_SPLITS}
    BANK_CONFLICT_COL_SPLITS=${SAMPLE_COL_SPLITS}
)
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)

# Keep host-side data generation in lockstep with the values compiled into the
# kernel.  The file is emitted in the build tree, which is also the expected
# working directory when running `python3 ../scripts/gen_data.py` and `./demo`.
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sample_config.json"
    CONTENT "{\"m\": ${SAMPLE_TOTAL_M}, \"n\": ${SAMPLE_TOTAL_N}, \"row_splits\": ${SAMPLE_ROW_SPLITS}, \"col_splits\": ${SAMPLE_COL_SPLITS}, \"scenario\": ${SCENARIO_NUM}}\n")
