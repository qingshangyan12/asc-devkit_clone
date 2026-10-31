# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
project(ascendc_utils_debug_tests)
cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 17)

add_executable(ascendc_ut_debug_api
    ${ASCENDC_DIR}/tests/main.cpp
    test_check_condition.cpp
)

target_include_directories(ascendc_ut_debug_api PRIVATE
    ${ASCENDC_DIR}/include
    ${ASCENDC_DIR}
)

target_compile_options(ascendc_ut_debug_api PRIVATE
    -Werror
)

target_link_libraries(ascendc_ut_debug_api PRIVATE
    $<BUILD_INTERFACE:intf_llt_pub_basic>
)

run_llt_test(
    TARGET ascendc_ut_debug_api
    TASK_NUM 1
)
