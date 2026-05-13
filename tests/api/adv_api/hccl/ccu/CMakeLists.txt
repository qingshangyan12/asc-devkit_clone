# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except be in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set(CCU_UT_TARGET ascendc_ut_adv_api_hccl_ccu_mc2)

set(HCCL_CC_DIR ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc)
set(HCCL_CC_SRC_DIR ${HCCL_CC_DIR}/src)
set(HCCL_CC_INCLUDE_DIR ${ASCENDC_DIR}/include/adv_api/hccl/internal)
set(HCCL_DEV_BASE ${HCCL_CC_DIR})
set(HCCL_LOCAL_PKG_INC ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc)
set(HCCL_ST_DIR ${ASCENDC_DIR}/tests/api/adv_api/hccl/cc/st/algorithm/utils/src)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(HCCL_CANN_ARCH_DIR ${ASCEND_CANN_PACKAGE_PATH}/x86_64-linux)
else()
    set(HCCL_CANN_ARCH_DIR ${ASCEND_CANN_PACKAGE_PATH}/aarch64-linux)
endif()

set(CCU_INCLUDE_LIST
    ${UT_COMMON_INCLUDE_LIST}
    ${CANN_3RD_LIB_PATH}/gtest/include
    ${ASCENDC_DIR}
    ${HCCL_CC_INCLUDE_DIR}
    ${HCCL_CC_INCLUDE_DIR}/hcomm
    ${HCCL_CC_INCLUDE_DIR}/hcomm/hccl
    ${HCCL_LOCAL_PKG_INC}
    ${HCCL_LOCAL_PKG_INC}/hccl
    ${HCCL_LOCAL_PKG_INC}/hcomm/ccu
    ${ASCEND_CANN_PACKAGE_PATH}/include
    ${ASCEND_CANN_PACKAGE_PATH}/include/acl
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/base
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/runtime
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/aicpu
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/mmpa
    ${HCCL_CANN_ARCH_DIR}/pkg_inc/runtime
    ${HCCL_CANN_ARCH_DIR}/pkg_inc/runtime/runtime
    ${HCCL_CC_SRC_DIR}
    ${HCCL_ST_DIR}/hccl_proxy
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${CCU_UT_TARGET}
    test_ccu_mc2.cpp
    # CCU kernel stub and necessary external dependency stubs from existing st framework
    ${HCCL_ST_DIR}/hccl_proxy/hcomm_ccu_stub.cc
    ${HCCL_ST_DIR}/hccl_proxy/log_stub.cc
    ${HCCL_ST_DIR}/hccl_proxy/mmpa_stub.cc
    # Source under test
    ${HCCL_CC_SRC_DIR}/common/hccl_mc2.cc
    # Real common infrastructure
    ${HCCL_CC_SRC_DIR}/common/utils.cc
    ${HCCL_CC_SRC_DIR}/common/log.cc
    ${HCCL_CC_SRC_DIR}/common/sal.cc
    ${HCCL_CC_SRC_DIR}/common/config_log.cc
    ${HCCL_CC_SRC_DIR}/common/adapter_error_manager_pub.cc
    ${HCCL_CC_SRC_DIR}/common/alg_env_config.cc
    ${HCCL_CC_SRC_DIR}/common/alg_type.cc
    # Real selector infrastructure
    ${HCCL_CC_SRC_DIR}/ops/op_common/selector/auto_selector_base.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/selector/selector_registry.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/selector/execute_selector.cc
    # Per-op selectors (register CCU algorithm selection logic)
    ${HCCL_CC_SRC_DIR}/ops/all_gather/selector/all_gather_auto_selector.cc
    ${HCCL_CC_SRC_DIR}/ops/all_reduce/selector/all_reduce_auto_selector.cc
    ${HCCL_CC_SRC_DIR}/ops/reduce_scatter/selector/reduce_scatter_auto_selector.cc
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/selector/alltoall_auto_selector.cc
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/selector/alltoallv_auto_selector.cc
    # Executor registry (for CollAlgExecRegistryV2)
    ${HCCL_CC_SRC_DIR}/ops/op_common/executor/registry/coll_alg_v2_exec_registry.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/executor/executor_v2_base.cc
    # Topo infrastructure
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo_match_base.cc
)

target_include_directories(${CCU_UT_TARGET} PRIVATE
    ${CCU_INCLUDE_LIST}
)

target_compile_options(${CCU_UT_TARGET} PRIVATE
    -std=c++17
    -O0
    -g
    -Wno-unused-variable
    -Wno-unused-parameter
    -Wno-attributes
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_definitions(${CCU_UT_TARGET} PRIVATE
    -DUT_TEST
    -DAICPU_COMPILE
)

target_link_libraries(${CCU_UT_TARGET} PRIVATE
    intf_llt_pub_basic
    c_sec
    pthread
    dl
)

run_llt_test(
    TARGET ${CCU_UT_TARGET}
    TASK_NUM 1
)
