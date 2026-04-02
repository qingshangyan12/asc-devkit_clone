# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set(HCCL_CC_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HCCL_CC_IMPL_DIR ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc)

file(GLOB HCCL_CC_UT_SRC_FILES
    ut/common/prepare_ut_env/main.cc
)

file(GLOB HCCL_CC_ST_SRC_FILES
    st/algorithm/testcase/main.cc
    st/algorithm/testcase/scatter_testcase.cc
    st/algorithm/testcase/all_reduce_testcase.cc
    st/algorithm/testcase/all_reduce_parallel_testcase.cc
    st/algorithm/testcase/all_gather_aicpu_testcase.cc
    st/algorithm/testcase/all_gather_v_testcase.cc
    st/algorithm/testcase/reduce_scatter_aicpu_testcase.cc
)

add_executable(ascendc_ut_cc_api_ascend950pr_9599
    ${HCCL_CC_UT_SRC_FILES}
    ${HCCL_CC_ST_SRC_FILES}
)

target_include_directories(ascendc_ut_cc_api_ascend950pr_9599 PRIVATE
    ${HCCL_CC_IMPL_DIR}/src/ops/op_common/inc
    ${HCCL_CC_IMPL_DIR}/src/common
    ${HCCL_CC_IMPL_DIR}/src/ops
    ${HCCL_CC_IMPL_DIR}/src/ops/op_common
    ${HCCL_CC_IMPL_DIR}/src/common/hcomm_dlsym
    ${HCCL_CC_TEST_DIR}/st/algorithm/utils/src/sim_world
    ${HCCL_CC_TEST_DIR}/st/algorithm/utils/src/common/utils
    ${HCCL_CC_TEST_DIR}/st/algorithm/utils/src/common/exception
    ${HCCL_CC_TEST_DIR}/st/algorithm/utils/src/hccl_verifier
    ${HCCL_CC_TEST_DIR}/st/algorithm/utils/src/hccl_verifier/semantics_check
    ${HCCL_CC_TEST_DIR}/st/algorithm/utils/src/hccl_verifier/task_graph_generator
    ${HCCL_CC_TEST_DIR}/st/algorithm/utils/src/hccl_proxy
    ${HCCL_CC_TEST_DIR}/st/algorithm/utils/src/hccl_proxy/communicator
    ${HCCL_CC_TEST_DIR}/st/algorithm/utils/src/hccl_proxy/topo_model
    ${ASCENDC_INCLUDE_DIR}/adv_api/hccl/internal
    ${ASCEND_CANN_PACKAGE_PATH}/include
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/base
    ${ASCEND_CANN_PACKAGE_PATH}/include/hccl
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/hccl
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/hcomm/ccu
)

target_compile_options(ascendc_ut_cc_api_ascend950pr_9599 PRIVATE
    -std=c++14
    -Werror
    -fno-common
    -fno-strict-aliasing
    -pipe
    -O0
    -g
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_link_libraries(ascendc_ut_cc_api_ascend950pr_9599 PRIVATE
    $<BUILD_INTERFACE:intf_llt_pub_basic>
    -Wl,--no-as-needed
    mmpa
    c_sec
    -Wl,--as-needed
)

run_llt_test(
    TARGET ascendc_ut_cc_api_ascend950pr_9599
    TASK_NUM 1
)
