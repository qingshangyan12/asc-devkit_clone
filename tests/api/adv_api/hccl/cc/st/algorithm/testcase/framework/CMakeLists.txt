# --------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# --------------------------------------------------------------------------------------------

# ==========================================================================================================
# Target : ST framework self-test
# Verifies IR capture, static checker, runtime, DAG checker, expectation — no product code needed
# ==========================================================================================================
set(HCCL_CCU_FRAMEWORK_SELFTEST_TARGET ascendc_ut_framework_selftest)

add_executable(
    ${HCCL_CCU_FRAMEWORK_SELFTEST_TARGET}
    ${ASCENDC_DIR}/tests/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framework_selftest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/checker_selftest.cpp
)

target_include_directories(
    ${HCCL_CCU_FRAMEWORK_SELFTEST_TARGET}
    PRIVATE
    ${UTILS_DIR}/hccl_proxy
    ${UTILS_DIR}/ccu_scenario
    ${UTILS_DIR}/hccl_verifier
    ${UTILS_DIR}/sim_world
    ${HCCL_CHECKER_COMMON_INCLUDE_LIST}
)

target_compile_options(
    ${HCCL_CCU_FRAMEWORK_SELFTEST_TARGET}
    PRIVATE
    -std=c++14
    -O0
    -g
    -fno-inline
    -Wno-unused-variable
    -Wno-unused-parameter
    -Wno-attributes
    -Wno-unknown-pragmas
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_definitions(${HCCL_CCU_FRAMEWORK_SELFTEST_TARGET} PRIVATE -DUT_TEST)

target_link_libraries(
    ${HCCL_CCU_FRAMEWORK_SELFTEST_TARGET}
    PRIVATE
    -Wl,--whole-archive
    hccl-model-proxy
    -Wl,--no-whole-archive
    hccl-verifier
    intf_llt_pub_basic
    c_sec
    pthread
    dl
)

run_llt_test(
    TARGET ${HCCL_CCU_FRAMEWORK_SELFTEST_TARGET}
    TASK_NUM 1
)
