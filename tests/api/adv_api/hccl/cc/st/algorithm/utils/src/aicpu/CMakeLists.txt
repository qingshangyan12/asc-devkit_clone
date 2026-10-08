# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set(INCLUDE_LIST
    ${HCCL_CC_INCLUDE_DIR}
    ${HCCL_DEV_BASE}/src/common
    ${HCCL_DEV_BASE}/src/common/hcomm_dlsym
    ${HCCL_DEV_BASE}/src/common/hcomm_dlsym/ccu

    ${HCCL_DEV_BASE}/src/ops
    ${HCCL_DEV_BASE}/src/ops/op_common
    ${HCCL_DEV_BASE}/src/ops/op_common/inc

    ${HCCL_DEV_BASE}/src/ops/op_common/executor
    ${HCCL_DEV_BASE}/src/ops/op_common/executor/channel
    ${HCCL_DEV_BASE}/src/ops/op_common/executor/registry

    ${HCCL_DEV_BASE}/src/ops/op_common/selector
    ${HCCL_DEV_BASE}/src/ops/op_common/external_alg

    ${HCCL_DEV_BASE}/src/ops/op_common/template
    ${HCCL_DEV_BASE}/src/ops/op_common/template/registry
    ${HCCL_DEV_BASE}/src/ops/op_common/template/aicpu
    ${HCCL_DEV_BASE}/src/ops/op_common/template/wrapper
    ${HCCL_DEV_BASE}/src/ops/op_common/template/aiv
    ${HCCL_DEV_BASE}/src/ops/op_common/template/ccu
    ${HCCL_DEV_BASE}/src/ops/op_common/template/ccu/kernel

    ${HCCL_DEV_BASE}/src/ops/op_common/topo

    ${HCCL_DEV_BASE}/src/ops/reduce_scatter
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/executor
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/selector
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aiv
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/ccu
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/ccu/kernel

    ${HCCL_DEV_BASE}/src/ops/all_gather
    ${HCCL_DEV_BASE}/src/ops/all_gather/executor
    ${HCCL_DEV_BASE}/src/ops/all_gather/selector
    ${HCCL_DEV_BASE}/src/ops/all_gather/template
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aicpu
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/ccu
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aiv

    ${HCCL_DEV_BASE}/src/ops/all_to_all_v
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/executor
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/selector
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/template
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/template/aicpu

    # R10 · F1 转交项（2026-10-08，ST agent 修复 UT 基建）：unified 头的 include
    # 传递闭包。kernel_launch.cc :25-26 include 了 hcomm_verb_sink.h（unified/l5）
    # 与 unified_aicpu_exec.h（unified/exec）——R6 起该 TU 消费 unified 头，而本
    # INCLUDE_LIST 此前无任何 unified 路径（.o 停在 8 月 27 日，该目标长期未重编）。
    # 与 impl 侧 src/CMakeLists.txt :108-123 的全局 INCLUDE_LIST 同口径：
    #   inc/  hcomm_verb_sink.h include "verb_sink.h"（词汇表半边在 inc/）
    #   l2/l3/l4  unified_aicpu_exec.h → aicpu_consumer.h 的闭包（l1_adapter/
    #         action_collect_all/backend_registry 等配方与动作头）
    #   exec/ l5/  kernel_launch.cc 的直连 include（:25-26）
    #   ../unified_exec  aicpu_consumer.h（消费方本体纯头）的所在目录
    # kfc_server 两条：op_common.cc :61 include "kernel/ccu_kernel_kfc_server.h"
    #（unified CCU 注册链的 M1 需要——路径形态与 testcase/mc2_ccu :159-160 同源）。
    # 本 CMakeLists 的三个静态库（selectors/executors/kernel）共用 INCLUDE_LIST，
    # 全部含 kernel_launch.cc/op_common.cc 或其闭包消费者，路径一次补齐对三目标生效。
    ${HCCL_DEV_BASE}/src/ops/unified/inc
    ${HCCL_DEV_BASE}/src/ops/unified/l2
    ${HCCL_DEV_BASE}/src/ops/unified/l3
    ${HCCL_DEV_BASE}/src/ops/unified/l4
    ${HCCL_DEV_BASE}/src/ops/unified/l5
    ${HCCL_DEV_BASE}/src/ops/unified/exec
    ${HCCL_DEV_BASE}/src/ops/unified_exec
    ${HCCL_DEV_BASE}/src/ops/kfc_server/template/ccu
    ${HCCL_DEV_BASE}/src/ops/kfc_server/template/ccu/kernel
    # kfc_server 头闭包补齐（R10 转交项修复续）：ccu_temp_kfc_server.cc :14-15 还
    # include 各算法 template 头（all_gather NHR multiJetty / all_to_all_v multiJetty）
    # ——这些头分散在各算法树的 template/ccu/ 下，与 testcase/mc2_ccu :146 的
    # include 路径集合同口径（该目标编同一 TU 从未缺路径——本目标长期未重编才漏）。
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/ccu
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/template/ccu

    ${HCCL_DEV_BASE}/src/common/hcomm_dlsym
    ${HCCL_DEV_BASE}/src/common/hcomm_dlsym/ccu
)

list(APPEND INCLUDE_LIST
    ${HCCL_CHECKER_COMMON_INCLUDE_LIST}
)

add_library(${HCCL_CHECKER_SELECTOR_TARGET} STATIC
    ${HCCL_DEV_BASE}/src/ops/all_gather/selector/all_gather_auto_selector.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/selector/reduce_scatter_auto_selector.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/selector/alltoall_auto_selector.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/selector/alltoallv_auto_selector.cc
)

target_include_directories(${HCCL_CHECKER_SELECTOR_TARGET} PRIVATE
    ${INCLUDE_LIST}
)

target_compile_options(${HCCL_CHECKER_SELECTOR_TARGET} PRIVATE
    -std=c++17
    -g
    -O0
    -fstack-protector-all
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_definitions(${HCCL_CHECKER_SELECTOR_TARGET} PRIVATE
    -DUT_TEST
    -DAICPU_COMPILE
)

add_library(${HCCL_CHECKER_EXECUTOR_TARGET} STATIC
    ${HCCL_DEV_BASE}/src/ops/op_common/topo/topo_match_pcie_mix.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/topo/topo_match_ubx_1d.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sole_executor.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/executor/ins_reduce_scatter_parallel_executor.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor_aicpu.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_nhr.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_meshchunk.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_Z_axis_detour.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_aicpu_reduce_nhr.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1d_dpu.cc

    ${HCCL_DEV_BASE}/src/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/executor/ins_v2_all_gather_parallel_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/executor/ins_v2_all_gather_sequence_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/executor/ins_v2_all_gather_sequence_executor_aicpu.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr_dpu.cc

    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/executor/ins_v2_all_to_all_v_sole_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/executor/ins_v2_all_to_all_concurrent_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/template/aicpu/ins_temp_all_to_all_v_mesh_1D.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/template/aicpu/ins_temp_ubx_all_to_all_v_mesh_1D.cc
)

target_include_directories(${HCCL_CHECKER_EXECUTOR_TARGET} PRIVATE
    ${INCLUDE_LIST}
)

target_compile_options(${HCCL_CHECKER_EXECUTOR_TARGET} PRIVATE
    -std=c++17
    -g
    -O0
    -fstack-protector-all
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_definitions(${HCCL_CHECKER_EXECUTOR_TARGET} PRIVATE
    -DUT_TEST
    -DAICPU_COMPILE
)

add_library(${HCCL_CHECKER_KERNEL_TARGET} STATIC
    ${HCCL_DEV_BASE}/src/common/utils.cc
    # ${HCCL_DEV_BASE}/src/common/adapter_acl.cc
    ${HCCL_DEV_BASE}/src/common/config_log.cc
    ${HCCL_DEV_BASE}/src/common/sal.cc
    ${HCCL_DEV_BASE}/src/common/log.cc
    ${HCCL_DEV_BASE}/src/common/adapter_error_manager_pub.cc
    ${HCCL_DEV_BASE}/src/common/alg_env_config.cc
    ${HCCL_DEV_BASE}/src/common/hcomm_dlsym/hcomm_primitives_dl.cc

    channel_stub.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/executor/channel/channel_request.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/executor/registry/coll_alg_exec_registry.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/executor/registry/coll_alg_v2_exec_registry.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/executor/executor_base.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/executor/executor_v2_base.cc

    ${HCCL_DEV_BASE}/src/ops/op_common/template/alg_template_base.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/template/alg_v2_template_base.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/template/template_utils.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/template/aicpu/kernel_launch.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/template/aicpu/dfx/task_exception_fun.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/template/aicpu/task_cache/mc2_aicpu_task_cache.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/template/registry/alg_template_register.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/template/registry/alg_v2_template_register.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/template/wrapper/alg_data_trans_wrapper.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/template/wrapper/dpu_alg_data_trans_wrapper.cc

    ${HCCL_DEV_BASE}/src/ops/op_common/topo/topo.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/topo/topo_match_base.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/topo/topo_match_pcie_mix.cc
    topo_match_aicpu_stub.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/topo/topo_match_ubx_1d.cc

    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sole_executor.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/executor/ins_reduce_scatter_parallel_executor.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor_aicpu.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_nhr.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_meshchunk.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_Z_axis_detour.cc

    ${HCCL_DEV_BASE}/src/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/executor/ins_v2_all_gather_parallel_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/executor/ins_v2_all_gather_sequence_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/executor/ins_v2_all_gather_sequence_executor_aicpu.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr_dpu.cc

    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/executor/ins_v2_all_to_all_v_sole_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/executor/ins_v2_all_to_all_concurrent_executor.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/template/aicpu/ins_temp_all_to_all_v_mesh_1D.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/template/aicpu/ins_temp_ubx_all_to_all_v_mesh_1D.cc
)

target_sources(${HCCL_CHECKER_KERNEL_TARGET} PRIVATE
    ${HCCL_DEV_BASE}/src/common/param_check.cc
    ${HCCL_DEV_BASE}/src/common/alg_type.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/op_common.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/cann_host_bridge.cc
    # R10 · F1 转交项（2026-10-08，ST agent 修复 UT 基建）：unified exec 薄壳——
    # kernel_launch.cc :373（R6 分叉）与 op_common.cc 的 unified 注册链调用
    # refactor::RunUnifiedAicpuOp（impl 本 .cc，M1/R6 起生产侧由 unified/CMakeLists
    # 编进 mc2_client；impl 侧 src/CMakeLists.txt :171-176 自 R10 起设备侧 mc2_server
    # 也编它）。UT 链路同缺：kernel_launch.cc.o 自 R6 起引用该符号，但本目标 .o
    # 停在 8 月 27 日（断链被掩盖），重编后 undefined reference 显形——随 INCLUDE_LIST
    # 的 unified 路径一并补源文件，链接闭环。
    # kfc_server 两条：op_common.cc :61 include "kernel/ccu_kernel_kfc_server.h" 且
    # extern 声明 InheritKfcServerKernelArg（定义在 ccu_temp_kfc_server.cc）、
    # CcuKfcServerKernel（定义在 ccu_kernel_kfc_server.cc）——M1 注册链的符号，
    # 生产侧同目标编译，UT 侧此前同样被陈旧 .o 掩盖。两条 .cc 与
    # testcase/mc2_ccu/CMakeLists.txt :66-68 同源（路径/文件一致）。
    ${HCCL_DEV_BASE}/src/ops/unified/exec/unified_aicpu_exec.cc
    ${HCCL_DEV_BASE}/src/ops/kfc_server/template/ccu/ccu_temp_kfc_server.cc
    ${HCCL_DEV_BASE}/src/ops/kfc_server/template/ccu/kernel/ccu_kernel_kfc_server.cc
    dlhcomm_function_stub.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/topo/topo_host.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/selector/auto_selector_base.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/selector/selector_registry.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/selector/execute_selector.cc
    ${HCCL_DEV_BASE}/src/ops/all_gather/selector/all_gather_auto_selector.cc
    ${HCCL_DEV_BASE}/src/ops/reduce_scatter/selector/reduce_scatter_auto_selector.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/selector/alltoall_auto_selector.cc
    ${HCCL_DEV_BASE}/src/ops/all_to_all_v/selector/alltoallv_auto_selector.cc
    ${HCCL_DEV_BASE}/src/ops/op_common/external_alg/alg_meta_registry.cc
)

target_include_directories(${HCCL_CHECKER_KERNEL_TARGET} PRIVATE
    ${INCLUDE_LIST}
)

target_compile_options(${HCCL_CHECKER_KERNEL_TARGET} PRIVATE
    -std=c++17
    -g
    -O0
    -fstack-protector-all
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_definitions(${HCCL_CHECKER_KERNEL_TARGET} PRIVATE
    -DUT_TEST
    -DAICPU_COMPILE
)
