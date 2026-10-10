# --------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# --------------------------------------------------------------------------------------------

# ==========================================================================================================
# Target : AICPU ST（AllGather 首期）
# 链路：InitOpParamByTiling → SelectAlgAndPrepareEngine → GetOpParamResCtx → HcclExecOp
#       → HcclLaunchAicpuKernel → Orchestrate → 模板 KernelRun → Hcomm* stub → TaskStub → SimTaskQueue
# 校验：静态检查 + 算法选择断言 + golden 逐字节比对 + TaskGraphExecutor 真执行/死锁定位 + DAG 内存冲突
# ==========================================================================================================
set(HCCL_AICPU_ST_TARGET ascendc_ut_adv_api_aicpu_st)
set(HCCL_MC2_AICPU_ALL_GATHER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/all_gather)
set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/src)
set(CC_SRC ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/src)

add_executable(
    ${HCCL_AICPU_ST_TARGET}
    ${ASCENDC_DIR}/tests/main.cpp
    aicpu_float_convert_test.cpp
    sim_backing_test.cpp
    task_graph_executor_test.cpp
    aicpu_fixture_test.cpp
    ${UTILS_DIR}/aicpu_scenario/task_graph_executor.cc
    ${UTILS_DIR}/aicpu_scenario/aicpu_fixture.cc
    ${HCCL_MC2_AICPU_ALL_GATHER_DIR}/all_gather_testcase.cpp
    ${HCCL_MC2_AICPU_ALL_GATHER_DIR}/aicpu_all_gather.cc
    ${HCCL_MC2_AICPU_ALL_GATHER_DIR}/aicpu_all_gather_mesh1d_adapter.cc
    ${CC_SRC}/common/hccl_alloc_ctx_res.cc
    ${CC_SRC}/ops/op_common/external_alg/external_alg_rules.cc
    ${CC_SRC}/ops/op_common/external_alg/external_alg_resolver.cc
    ${CC_SRC}/ops/op_common/topo/physical_level_build.cc
    ${CC_SRC}/ops/op_common/topo/physical_level_normalize.cc
)

target_include_directories(
    ${HCCL_AICPU_ST_TARGET}
    PRIVATE
    ${HCCL_CHECKER_COMMON_INCLUDE_LIST}
    ${UTILS_DIR}/sim_world
    ${UTILS_DIR}/common/utils
    ${UTILS_DIR}/aicpu_scenario
    ${HCCL_MC2_AICPU_ALL_GATHER_DIR}
    ${CC_SRC}/common
    ${CC_SRC}/ops/op_common
    ${CC_SRC}/ops/op_common/inc
    ${CC_SRC}/ops/op_common/template/wrapper
)

target_compile_options(
    ${HCCL_AICPU_ST_TARGET}
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

target_compile_definitions(${HCCL_AICPU_ST_TARGET} PRIVATE -DUT_TEST -DAICPU_COMPILE)

target_link_libraries(
    ${HCCL_AICPU_ST_TARGET}
    PRIVATE
    intf_llt_pub_basic
    -Wl,--whole-archive
    ${HCCL_CHECKER_SELECTOR_TARGET}
    ${HCCL_CHECKER_EXECUTOR_TARGET}
    -Wl,--no-whole-archive
    -Wl,--start-group
    ${HCCL_CHECKER_KERNEL_TARGET}
    ${HCCL_CHECKER_SELECTOR_TARGET}
    ${HCCL_CHECKER_EXECUTOR_TARGET}
    ${HCCL_CHECKER_MODEL_PROXY_TARGET}
    -Wl,--end-group
    c_sec
    pthread
    dl
)
