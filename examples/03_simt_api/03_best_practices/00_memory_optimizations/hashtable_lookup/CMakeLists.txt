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

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU ARCH, e.g. dav-3510")
set(CMAKE_ASC_ENABLE_SIMT ON)

find_package(ASC REQUIRED)
project(hashtable_lookup LANGUAGES ASC CXX)


set(SCENARIO_NUM "3" CACHE STRING "Case type to run (0-3)")
if(NOT SCENARIO_NUM MATCHES "^[0-3]$")
    message(FATAL_ERROR "SCENARIO_NUM must be one of 0, 1, 2, 3, but got ${SCENARIO_NUM}")
endif()

set(LOAD_FACTOR "0.9" CACHE STRING "Table load factor, e.g. 0.5-0.9")
if(NOT LOAD_FACTOR MATCHES "^0\.[0-9]+$")
    message(FATAL_ERROR "LOAD_FACTOR must be a decimal in (0,1), but got ${LOAD_FACTOR}")
endif()

add_executable(hashtable_lookup
    hashtable_lookup.asc
)

target_compile_definitions(hashtable_lookup PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
    LOAD_FACTOR=${LOAD_FACTOR}
)

target_compile_options(hashtable_lookup PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--cce-res-usage>
)

target_link_libraries(hashtable_lookup PRIVATE
    m
)
