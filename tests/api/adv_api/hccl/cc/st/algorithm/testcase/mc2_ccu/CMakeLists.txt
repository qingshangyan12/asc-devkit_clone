# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except be in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set(HCCL_MC2_CCU_ST_DIR ${UTILS_DIR}/mc2_ccu)
set(HCCL_MC2_CCU_DEVICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/device)
set(HCCL_ALL_TO_ALL_V_CCU_KERNEL_DIR
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/template/ccu/kernel)
set(HCCL_KFC_SERVER_CCU_KERNEL_DIR
    ${HCCL_CC_SRC_DIR}/ops/kfc_server/template/ccu/kernel)

add_executable(${HCCL_MC2_CCU_HOST_TARGET}
    ${ASCENDC_DIR}/tests/main.cpp
    host/all_to_all_v_mc2_host_testcase.cpp
    ${HCCL_MC2_CCU_ST_DIR}/cann_host_bridge_stub.cc
    ${HCCL_MC2_CCU_ST_DIR}/hccl_op_common_stub.cc
    ${HCCL_CC_SRC_DIR}/common/hccl_mc2.cc
    ${HCCL_CC_SRC_DIR}/common/hccl_alloc_ctx_res.cc
)

target_include_directories(${HCCL_MC2_CCU_HOST_TARGET} PRIVATE
    ${HCCL_CHECKER_COMMON_INCLUDE_LIST}
    ${HCCL_CC_INCLUDE_DIR}/hcomm/types
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/profiling
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/toolchain
    ${HCCL_CC_SRC_DIR}
    ${HCCL_CC_SRC_DIR}/aicpu_kfc/legacy/common/types
    ${HCCL_CC_SRC_DIR}/aicpu_kfc/legacy/common/utils
    ${HCCL_CC_SRC_DIR}/aicpu_kfc/legacy/common/exception
    ${HCCL_MC2_CCU_ST_DIR}
)

target_compile_options(${HCCL_MC2_CCU_HOST_TARGET} PRIVATE
    -std=c++17
    -O0
    -g
    -Wno-unused-variable
    -Wno-unused-parameter
    -Wno-attributes
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_definitions(${HCCL_MC2_CCU_HOST_TARGET} PRIVATE
    -DUT_TEST
    -DAICPU_COMPILE
    -DMC2_CLIENT_ENABLE_CCU=1
)

target_link_libraries(${HCCL_MC2_CCU_HOST_TARGET} PRIVATE
    hccl-model-proxy
    intf_llt_pub_basic
    c_sec
    pthread
    dl
)

run_llt_test(
    TARGET ${HCCL_MC2_CCU_HOST_TARGET}
    TASK_NUM 1
)

add_executable(${HCCL_MC2_CCU_DEVICE_TARGET}
    ${ASCENDC_DIR}/tests/main.cpp
    device/all_to_all_v_kernel_device_testcase.cpp
    device/all_to_all_v_kfc_device_testcase.cpp
    device/all_to_all_link_stub.cc
    ${UTILS_DIR}/ccu/ccu_all_to_all_v_mesh1d_adapter.cc
    ${UTILS_DIR}/ccu/ccu_kfc_server_adapter.cc
    ${HCCL_ALL_TO_ALL_V_CCU_KERNEL_DIR}/ccu_kernel_all_to_all_v_mesh1d.cc
    ${HCCL_KFC_SERVER_CCU_KERNEL_DIR}/ccu_kernel_kfc_server.cc
)

target_include_directories(${HCCL_MC2_CCU_DEVICE_TARGET} PRIVATE
    ${HCCL_MC2_CCU_DEVICE_DIR}/shim
    ${HCCL_CHECKER_COMMON_INCLUDE_LIST}
    ${HCCL_ALL_TO_ALL_V_CCU_KERNEL_DIR}
    ${HCCL_KFC_SERVER_CCU_KERNEL_DIR}
)

target_compile_options(${HCCL_MC2_CCU_DEVICE_TARGET} PRIVATE
    -std=c++14
    -Werror
    -fno-common
    -fno-strict-aliasing
    -O0
    -g
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_definitions(${HCCL_MC2_CCU_DEVICE_TARGET} PRIVATE
    -DUT_TEST
)

target_link_libraries(${HCCL_MC2_CCU_DEVICE_TARGET} PRIVATE
    intf_llt_pub_basic
    hccl-model-proxy
    hccl-verifier
    c_sec
    pthread
    dl
)

run_llt_test(
    TARGET ${HCCL_MC2_CCU_DEVICE_TARGET}
    TASK_NUM 1
)
