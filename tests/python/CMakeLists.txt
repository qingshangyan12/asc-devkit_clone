# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
project(ascendc_python_tests)
cmake_minimum_required(VERSION 3.16.0)
add_subdirectory(ascendc_hidden)

set(INSTALL_PATH ${CMAKE_BINARY_DIR})
set(DST_PATH adapter_ut)

add_custom_target(generate_hidden_dat_file_ut ALL
    COMMAND code_hidden ${ASCENDC_DIR}/tests/python/adapter/stub_kernels ${ASCENDC_DIR}/tests/python/adapter/stub_kernels/ascendc_impl.dat
    DEPENDS code_hidden
)

add_custom_target(install_adapter_api_ut ALL
    COMMAND bash ${ASCENDC_DIR}/tests/python/ci/install_files.sh ${ASCENDC_DIR} ${INSTALL_PATH} ${DST_PATH}
)

run_python_llt_test(
    TARGET ascendc_aclrt_launch_kernel_python_ut
    SRC_FILES_DIR ${ASCENDC_DIR}/cmake/
    TEST_FILES_DIR ${ASCENDC_DIR}/tests/python/aclrt_launch_kernel/
    PRE_TEST_COMMAND
    COVERAGERC_DIR
)

run_python_llt_test(
    TARGET asc_op_compiler_python_ut
    SRC_FILES_DIR ${ASCENDC_DIR}/tools/build/asc_op_compile_base/asc_op_compiler/
    TEST_FILES_DIR ${ASCENDC_DIR}/tests/python/asc_op_compiler/
    ENV_FILE ${ASCENDC_DIR}/tests/python/ci/set_python_env.sh
    ENV_PARAMS ${ASCENDC_DIR}\ py_ut \ 910
    PRE_TEST_COMMAND
    COVERAGERC_DIR
)

add_dependencies(asc_op_compiler_python_ut
    register
    gen_kernel_tiling
    install_adapter_api_ut
    generate_hidden_dat_file_ut
    asc_platform_test
)

run_python_llt_test(
    TARGET tikcpp_utest_compile_trace_log
    SRC_FILES_DIR ${ASCENDC_DIR}/tools/scripts/compile_trace_log
    TEST_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/compile_trace_log
    EXPORT_PYTHONPATH ${CMAKE_CURRENT_SOURCE_DIR}/stub/
    PRE_TEST_COMMAND
    COVERAGERC_DIR
)