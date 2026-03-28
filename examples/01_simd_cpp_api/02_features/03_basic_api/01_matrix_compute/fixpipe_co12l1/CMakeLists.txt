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

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    src/fixpipe_co12l1.asc
)

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    m
)

# 用户可配置选项
option(INPUT_TYPE "input data type: half or int8" "half")
option(OUTPUT_TYPE "output data type: int8 or half" "int8")
option(PRE_QUANT_MODE "quantization mode: 0=none, 1=scalar, 2=vector" "0")
option(PRE_RELU_MODE "enable norm relu" "false")
option(EN_UNIT_FLAG "enable unitflag for mmad and fixpipe parallelism" "false")
option(USE_DATA_COPY "use DataCopy instead of Fixpipe" "false")

# 转换为编译宏
if (INPUT_TYPE STREQUAL "half")
    add_definitions(-DINPUT_TYPE_IS_HALF)
elseif (INPUT_TYPE STREQUAL "int8")
    add_definitions(-DINPUT_TYPE_IS_INT8)
else()
    message(FATAL_ERROR "Invalid INPUT_TYPE: ${INPUT_TYPE}, must be half or int8")
endif()

if (OUTPUT_TYPE STREQUAL "int8")
    add_definitions(-DOUTPUT_TYPE_IS_INT8)
elseif (OUTPUT_TYPE STREQUAL "half")
    add_definitions(-DOUTPUT_TYPE_IS_HALF)
else()
    message(FATAL_ERROR "Invalid OUTPUT_TYPE: ${OUTPUT_TYPE}, must be int8 or half")
endif()

if (PRE_QUANT_MODE STREQUAL "1")
    add_definitions(-DPRE_QUANT_MODE_SCALAR)
elseif (PRE_QUANT_MODE STREQUAL "2")
    add_definitions(-DPRE_QUANT_MODE_VECTOR)
endif()

if (PRE_RELU_MODE)
    add_definitions(-DPRE_RELU_MODE_ENABLE)
endif()

if (EN_UNIT_FLAG)
    add_definitions(-DEN_UNIT_FLAG)
endif()

if (USE_DATA_COPY)
    add_definitions(-DUSE_DATA_COPY)
endif()

message(STATUS "Configuration:")
message(STATUS "  INPUT_TYPE: ${INPUT_TYPE}")
message(STATUS "  OUTPUT_TYPE: ${OUTPUT_TYPE}")
message(STATUS "  PRE_QUANT_MODE: ${PRE_QUANT_MODE}")
message(STATUS "  PRE_RELU_MODE: ${PRE_RELU_MODE}")
message(STATUS "  EN_UNIT_FLAG: ${EN_UNIT_FLAG}")
message(STATUS "  USE_DATA_COPY: ${USE_DATA_COPY}")

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
)