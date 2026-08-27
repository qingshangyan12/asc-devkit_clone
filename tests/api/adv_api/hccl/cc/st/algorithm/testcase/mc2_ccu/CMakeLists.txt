# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# ==========================================================================================================
# Target : selector / CalcRes / channel full-chain ST
# Compiles real selector / executor / template + kernel, verifies full chain
# ==========================================================================================================
set(HCCL_CCU_ST_TARGET ascendc_ut_adv_api_ccu_st)
set(HCCL_MC2_CCU_ALL_TO_ALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/all_to_all)

set(HCCL_SELECTOR_PRODUCT_SOURCES
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/selector/alltoall_auto_selector.cc
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/executor/ins_v2_all_to_all_v_sole_executor.cc
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/template/ccu/ccu_temp_all_to_all_mesh_1D.cc
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/template/ccu/kernel/ccu_kernel_all_to_all_mesh1d.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/op_common.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/selector/auto_selector_base.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/selector/execute_selector.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/selector/selector_registry.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/executor/executor_v2_base.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/executor/registry/coll_alg_v2_exec_registry.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/executor/registry/coll_alg_exec_registry.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/template/ccu_alg_template_base.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/template/alg_v2_template_base.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/template/alg_template_base.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/template/template_utils.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/template/ccu/ccu_kernel_alg_base.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/template/ccu/ccu_kernel_utils.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/executor/channel/channel.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/executor/channel/channel_request.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo_host.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo_match_1d.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo_match_base.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo_match_multilevel.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo_match_ubx.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo_match_ubx_1d.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/topo/topo_match_pcie_mix.cc
    ${HCCL_CC_SRC_DIR}/common/hccl_alloc_ctx_res.cc
    ${HCCL_CC_SRC_DIR}/common/hccl_mc2.cc
    ${HCCL_CC_SRC_DIR}/ops/op_common/ccu_log.cc
    ${HCCL_CC_SRC_DIR}/common/hcomm_dlsym/ccu_res_dl.cc
    ${HCCL_CC_SRC_DIR}/common/hcomm_dlsym/hccl_ccu_res_dl.cc
)

add_executable(
    ${HCCL_CCU_ST_TARGET}
    ${ASCENDC_DIR}/tests/main.cpp
    ${HCCL_MC2_CCU_ALL_TO_ALL_DIR}/all_to_all_testcase.cpp
    ${HCCL_MC2_CCU_ALL_TO_ALL_DIR}/ccu_all_to_all_mesh1d_adapter.cc
    ${HCCL_MC2_CCU_ALL_TO_ALL_DIR}/ccu_all_to_all.cc
    ${UTILS_DIR}/ccu_scenario/ccu_fixture.cc
    ${HCCL_SELECTOR_PRODUCT_SOURCES}
)

target_include_directories(
    ${HCCL_CCU_ST_TARGET} PRIVATE
    ${UTILS_DIR}/hccl_proxy
    ${HCCL_MC2_CCU_ALL_TO_ALL_DIR}
    ${HCCL_CHECKER_COMMON_INCLUDE_LIST}
    ${UTILS_DIR}/ccu_scenario
    ${HCCL_CC_SRC_DIR}
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/selector
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/executor
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/template/ccu
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/template/ccu/kernel
    ${HCCL_CC_INCLUDE_DIR}/hcomm/types
    ${HCCL_CC_INCLUDE_DIR}/hcomm/ccu
    ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/runtime/runtime
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/profiling
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/toolchain
)

target_compile_options(
    ${HCCL_CCU_ST_TARGET} PRIVATE
    -std=c++14 -O0 -g -fno-inline -fprofile-update=atomic
    -Wno-unused-variable -Wno-unused-parameter -Wno-attributes -Wno-unknown-pragmas
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_definitions(${HCCL_CCU_ST_TARGET} PRIVATE -DUT_TEST -DMC2_CLIENT_ENABLE_CCU=1)

target_link_libraries(
    ${HCCL_CCU_ST_TARGET} PRIVATE
    -Wl,--whole-archive
    hccl-model-proxy
    -Wl,--no-whole-archive
    ${HCCL_CHECKER_KERNEL_TARGET}
    hccl-verifier
    intf_llt_pub_basic c_sec pthread dl
)

# AllToAllV MC2 CCU coverage uses the final ST framework above while keeping
# the product algorithm and KFC server kernel from the implementation patch.
set(HCCL_MC2_CCU_ST_DIR ${UTILS_DIR}/mc2_ccu)
set(HCCL_MC2_CCU_DEVICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/device)
set(HCCL_ALL_TO_ALL_V_CCU_KERNEL_DIR
    ${HCCL_CC_SRC_DIR}/ops/all_to_all_v/template/ccu/kernel)
set(HCCL_KFC_SERVER_CCU_KERNEL_DIR
    ${HCCL_CC_SRC_DIR}/ops/kfc_server/template/ccu/kernel)

set(HCCL_MC2_CCU_ALLTOALLV_HOST_TARGET ascendc_ut_adv_api_hccl_ccu_mc2_host)
add_executable(${HCCL_MC2_CCU_ALLTOALLV_HOST_TARGET}
    ${ASCENDC_DIR}/tests/main.cpp
    host/all_to_all_v_mc2_host_testcase.cpp
    ${HCCL_MC2_CCU_ST_DIR}/cann_host_bridge_stub.cc
    ${HCCL_MC2_CCU_ST_DIR}/hccl_op_common_stub.cc
    ${HCCL_CC_SRC_DIR}/common/hccl_mc2.cc
    ${HCCL_CC_SRC_DIR}/common/hccl_alloc_ctx_res.cc
)
target_include_directories(${HCCL_MC2_CCU_ALLTOALLV_HOST_TARGET} PRIVATE
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
target_compile_options(${HCCL_MC2_CCU_ALLTOALLV_HOST_TARGET} PRIVATE
    -std=c++17 -O0 -g -Wno-unused-variable -Wno-unused-parameter
    -Wno-attributes -D_GLIBCXX_USE_CXX11_ABI=0
)
target_compile_definitions(${HCCL_MC2_CCU_ALLTOALLV_HOST_TARGET} PRIVATE
    -DUT_TEST -DAICPU_COMPILE -DMC2_CLIENT_ENABLE_CCU=1
)
target_link_libraries(${HCCL_MC2_CCU_ALLTOALLV_HOST_TARGET} PRIVATE
    hccl-model-proxy intf_llt_pub_basic c_sec pthread dl
)
run_llt_test(TARGET ${HCCL_MC2_CCU_ALLTOALLV_HOST_TARGET} TASK_NUM 1)

set(HCCL_MC2_CCU_ALLTOALLV_DEVICE_TARGET ascendc_ut_adv_api_hccl_ccu_mc2_device)
add_executable(${HCCL_MC2_CCU_ALLTOALLV_DEVICE_TARGET}
    ${ASCENDC_DIR}/tests/main.cpp
    device/all_to_all_v_kernel_device_testcase.cpp
    device/all_to_all_v_kfc_device_testcase.cpp
    device/all_to_all_link_stub.cc
    ${UTILS_DIR}/ccu/ccu_all_to_all_v_mesh1d_adapter.cc
    ${UTILS_DIR}/ccu/ccu_kfc_server_adapter.cc
    ${HCCL_ALL_TO_ALL_V_CCU_KERNEL_DIR}/ccu_kernel_all_to_all_v_mesh1d.cc
    ${HCCL_KFC_SERVER_CCU_KERNEL_DIR}/ccu_kernel_kfc_server.cc
)
target_include_directories(${HCCL_MC2_CCU_ALLTOALLV_DEVICE_TARGET} PRIVATE
    ${HCCL_MC2_CCU_DEVICE_DIR}/shim
    ${HCCL_CHECKER_COMMON_INCLUDE_LIST}
    ${HCCL_ALL_TO_ALL_V_CCU_KERNEL_DIR}
    ${HCCL_KFC_SERVER_CCU_KERNEL_DIR}
)
target_compile_options(${HCCL_MC2_CCU_ALLTOALLV_DEVICE_TARGET} PRIVATE
    -std=c++14 -Werror -fno-common -fno-strict-aliasing -O0 -g
    -D_GLIBCXX_USE_CXX11_ABI=0
)
target_compile_definitions(${HCCL_MC2_CCU_ALLTOALLV_DEVICE_TARGET} PRIVATE -DUT_TEST)
target_link_libraries(${HCCL_MC2_CCU_ALLTOALLV_DEVICE_TARGET} PRIVATE
    intf_llt_pub_basic hccl-model-proxy hccl-verifier c_sec pthread dl
)
run_llt_test(TARGET ${HCCL_MC2_CCU_ALLTOALLV_DEVICE_TARGET} TASK_NUM 1)
