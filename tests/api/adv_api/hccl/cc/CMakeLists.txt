# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16.0)
project(hccl_cc_tests)

set(CC_IMPL_DIR ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc)
set(CC_UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)
set(CC_TESTCASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# ======================== Test utilities library ========================
add_library(hccl-model-proxy SHARED "")
add_subdirectory(utils/hccl_verifier)
add_subdirectory(utils/sim_world)
add_subdirectory(utils/hccl_proxy)
add_subdirectory(utils/common)
add_subdirectory(utils/hccl_depends_stub)
add_subdirectory(utils/aicpu)

target_compile_options(hccl-model-proxy PRIVATE
    -std=c++14
    -Werror
    -fno-common
    -fno-strict-aliasing
    -pipe
    -O0
    -g
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_link_libraries(hccl-model-proxy PUBLIC
    hcomm
    unified_dlog
    mmpa
    acl_rt
    mc2_server
)

if(BUILD_OPEN_PROJECT)
    target_link_libraries(hccl-model-proxy PUBLIC
        ${ASCEND_HOME_PATH}/lib64/libc_sec.so
    )

    target_include_directories(hccl-model-proxy PRIVATE
        ${CC_UTILS_DIR}/sim_world
        ${CC_UTILS_DIR}/common/exception
        ${CC_UTILS_DIR}/common/utils
        ${CC_UTILS_DIR}/hccl_verifier
        ${CC_UTILS_DIR}/hccl_proxy
        ${CC_UTILS_DIR}/hccl_proxy/communicator
        ${CC_UTILS_DIR}/hccl_proxy/topo_model

        # Ascend package headers
        ${ASCEND_HOME_PATH}/include
        ${ASCEND_HOME_PATH}/pkg_inc
        ${ASCEND_HOME_PATH}/pkg_inc/base
        ${ASCEND_HOME_PATH}/pkg_inc/runtime
        ${ASCEND_HOME_PATH}/pkg_inc/profiling
        ${ASCEND_HOME_PATH}/hcomm/include/hccl
        ${ASCEND_HOME_PATH}/include/hccl
        ${ASCEND_HOME_PATH}/include/hcomm
        ${ASCEND_HOME_PATH}/pkg_inc/hccl
        ${ASCEND_HOME_PATH}/pkg_inc/hcomm/ccu
        ${ASCEND_HOME_PATH}/include/mmpa

        # Source code headers
        ${CC_IMPL_DIR}/common
        ${CC_IMPL_DIR}/common/hcomm_dlsym
        ${CC_IMPL_DIR}/ops
        ${CC_IMPL_DIR}/ops/inc
        ${CC_IMPL_DIR}/ops/op_common/inc
    )
endif()

# ======================== Merged test cases (st + ut) ========================
# All test cases from both st and ut are combined into a single target
set(CC_TEST_SRC_FILES
    main.cc
    scatter_testcase.cc
    scatter_testcase_a3.cc
    all_reduce_testcase.cc
    all_reduce_parallel_testcase.cc
    all_reduce_dpu_testcase.cc
    all_gather_aicpu_testcase.cc
    all_gather_dpu_testcase.cc
    all_gather_v_testcase.cc
    alltoall_testcase.cc
    alltoallv_testcase.cc
    alltoallvc_testcase.cc
    batch_send_recv_testcase.cc
    broadcast_testcase.cc
    broadcast_dpu_testcase.cc
    dpu_sendrecv_testcase.cc
    reduce_scatter_aicpu_testcase.cc
    reduce_scatter_testcase.cc
    reduce_scatter_v_testcase.cc
    reduce_testcase.cc
    send_recv_testcase.cc
    # UT test cases
    ut_testcase/scatter_testcase.cc
)

add_executable(ascendc_ut_adv_api_ascend950pr_9599 ${CC_TEST_SRC_FILES})

if(BUILD_OPEN_PROJECT)
    set(ASCEND_HOME_PATH $ENV{ASCEND_HOME_PATH})
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        set(ASCEND_HOME_PATH "${ASCEND_HOME_PATH}/x86_64-linux")
    else()
        set(ASCEND_HOME_PATH "${ASCEND_HOME_PATH}/aarch64-linux")
    endif()

    target_include_directories(ascendc_ut_adv_api_ascend950pr_9599 PUBLIC
        # Third-party headers
        ${CANN_3RD_LIB_PATH}/gtest/include

        # HCCL source headers
        ${CC_IMPL_DIR}/ops/op_common/inc

        # Ascend package headers
        ${ASCEND_HOME_PATH}/include
        ${ASCEND_HOME_PATH}/pkg_inc
        ${ASCEND_HOME_PATH}/pkg_inc/base
        ${ASCEND_HOME_PATH}/hcomm/include/hccl
        ${ASCEND_HOME_PATH}/include/hccl
        ${ASCEND_HOME_PATH}/pkg_inc/hccl
        ${ASCEND_HOME_PATH}/pkg_inc/hcomm/ccu

        # Source code headers
        ${CC_IMPL_DIR}/common
        ${CC_IMPL_DIR}/ops
        ${CC_IMPL_DIR}/ops/op_common
        ${CC_IMPL_DIR}/ops/op_common/inc
        ${CC_IMPL_DIR}/common/hcomm_dlsym

        # Test utility headers
        ${CC_UTILS_DIR}/sim_world
        ${CC_UTILS_DIR}/common/utils
        ${CC_UTILS_DIR}/common/exception
        ${CC_UTILS_DIR}/hccl_verifier
        ${CC_UTILS_DIR}/hccl_verifier/semantics_check
        ${CC_UTILS_DIR}/hccl_verifier/task_graph_generator
        ${CC_UTILS_DIR}/hccl_proxy
        ${CC_UTILS_DIR}/hccl_proxy/communicator
        ${CC_UTILS_DIR}/hccl_proxy/topo_model
    )

    target_link_libraries(ascendc_ut_adv_api_ascend950pr_9599 PRIVATE
        gtest
        pthread
        hccl-model-proxy
        hccl-verifier
        # Link mc2_client library (formerly hccl)
        ${ASCEND_HOME_PATH}/lib64/libmc2_client.so
    )

    target_compile_options(ascendc_ut_adv_api_ascend950pr_9599 PRIVATE
        -std=c++14
        -Werror
        -fno-common
        -fno-strict-aliasing
        -pipe
        -O0
        -g
        -D_GLIBCXX_USE_CXX11_ABI=0
    )
endif()

run_llt_test(
    TARGET ascendc_ut_adv_api_ascend950pr_9599
    TASK_NUM 1
)
