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
    ${ASCENDC_DIR}/include/adv_api/hccl/internal
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common/hcomm_dlsym

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/inc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/executor/channel
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/executor/registry

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/selector

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/registry
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/aicpu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/wrapper
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/aiv
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/ccu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/ccu/kernel

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/topo

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/template/aicpu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/template/aiv
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/template/ccu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/template/ccu/kernel

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/template/aicpu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/template/ccu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/template/ccu/kernel
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/template/aiv

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter_v
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter_v/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter_v/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter_v/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter_v/template/aicpu

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/template/aicpu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/template/ccu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/template/ccu/kernel
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/template/aiv

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/template/aicpu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/template/ccu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/template/aiv

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather_v
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather_v/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather_v/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather_v/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather_v/template/aicpu

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v/template/aicpu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v/template/ccu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v/template/aiv

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/ccu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aiv

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/send
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/send/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/send/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/send/template

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/recv
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/recv/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/recv/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/recv/template

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/template
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/template/aicpu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/template/ccu
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/template/aiv

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/batch_send_recv
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/batch_send_recv/executor
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/batch_send_recv/selector
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common/hcomm_dlsym
)

list(APPEND INCLUDE_LIST
    ${ASCEND_HOME_PATH}/include
    ${ASCEND_HOME_PATH}/include/acl

    # hcomm头文件
    ${ASCEND_HOME_PATH}/include/hccl
    ${ASCEND_HOME_PATH}/include/hcomm

    # runtime头文件
    ${ASCEND_HOME_PATH}/include/
    ${ASCEND_HOME_PATH}/include/base/
    ${ASCEND_HOME_PATH}/include/driver/
    ${ASCEND_HOME_PATH}/include/dump/
    ${ASCEND_HOME_PATH}/include/external/
    ${ASCEND_HOME_PATH}/include/platform/

    # mmpa头文件
    ${ASCEND_HOME_PATH}/pkg_inc/mmpa/

    # 包间接口
    ${ASCEND_HOME_PATH}/pkg_inc/
    ${ASCEND_HOME_PATH}/pkg_inc/hccl
    ${ASCEND_HOME_PATH}/pkg_inc/hcomm/ccu
    ${ASCEND_HOME_PATH}/pkg_inc/runtime/
    ${ASCEND_HOME_PATH}/pkg_inc/aicpu/
    ${ASCEND_HOME_PATH}/pkg_inc/runtime/runtime/
    ${ASCEND_HOME_PATH}/pkg_inc/profiling/
    ${ASCEND_HOME_PATH}/pkg_inc/base/
    ${ASCEND_HOME_PATH}/pkg_inc/dump/
    ${ASCEND_HOME_PATH}/pkg_inc/trace/
)

add_library(mc2_server SHARED
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common/utils.cc
    # ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common/adapter_acl.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common/config_log.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common/sal.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common/log.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common/adapter_error_manager_pub.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/common/alg_env_config.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/executor/channel/channel.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/executor/channel/channel_request.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/executor/registry/coll_alg_exec_registry.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/executor/registry/coll_alg_v2_exec_registry.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/executor/executor_base.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/executor/executor_v2_base.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/alg_template_base.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/alg_v2_template_base.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/template_utils.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/aicpu/kernel_launch.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/aicpu/dfx/task_exception_fun.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/registry/alg_template_register.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/registry/alg_v2_template_register.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/wrapper/alg_data_trans_wrapper.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/template/wrapper/dpu_alg_data_trans_wrapper.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/topo/topo.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/topo/topo_match_1d.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/topo/topo_match_base.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/topo/topo_match_multilevel.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/op_common/topo/topo_match_ubx.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/scatter_comm_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/scatter_executor_base.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/scatter_mesh_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/scatter_ring_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/scatter_single_executor.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/template/nhr_base.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/template/scatter_mesh.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/template/scatter_nb.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/template/scatter_nhr.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/template/scatter_ring_direct.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/algo/template/scatter_ring.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/executor/ins_v2_scatter_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/executor/ins_v2_scatter_parallel_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/template/aicpu/ins_temp_scatter_mesh_1D.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/scatter/template/aicpu/ins_temp_scatter_nhr.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/executor/ins_reduce_scatter_parallel_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_nhr.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_meshchunk.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/executor/ins_v2_all_gather_parallel_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/executor/ins_v2_all_gather_sequence_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr_dpu.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu/ins_temp_all_gather_mesh_1D_intra.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu/ins_temp_all_gather_nhr_dpu_inter.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu/ins_temp_reduce_scatter_mesh_1D_dpu_inter.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu/ins_temp_reduce_scatter_mesh_1D_intra.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather_v/executor/ins_v2_all_gather_v_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_gather_v/template/aicpu/ins_temp_all_gather_v_mesh_1D.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v/executor/ins_v2_all_to_all_v_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v/template/aicpu/ins_temp_all_to_all_v_mesh_1D.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_to_all_v/template/aicpu/ins_temp_dpu_alltoall_mesh.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/send/executor/ins_v2_send_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/send/executor/ins_send_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/send/template/ins_temp_send_dpu.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/recv/executor/ins_v2_recv_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/recv/executor/ins_recv_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/recv/template/ins_temp_recv_dpu.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/executor/ins_all_reduce_parallel_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/executor/ins_v2_all_reduce_sequence_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_one_shot.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu/ins_temp_all_reduce_nhr.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/executor/ins_v2_broadcast_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/executor/ins_v2_broadcast_parallel_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/template/aicpu/ins_temp_broadcast_mesh_1D_two_shot.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/broadcast/template/aicpu/ins_temp_broadcast_nhr.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/batch_send_recv/executor/ins_v2_batch_send_recv_executor.cc

    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/executor/reduce_sole_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/executor/reduce_parallel_executor.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/template/aicpu/reduce_mesh_1D.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/template/aicpu/reduce_nhr.cc
    ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/ops/reduce/template/aicpu/reduce_aicpu_reduce_nhr.cc
)

target_include_directories(mc2_server PRIVATE
    ${INCLUDE_LIST}
)

target_compile_options(mc2_server PRIVATE
    -g
    -O0
    -fstack-protector-all
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_link_options(mc2_server PRIVATE
    -Wl,-z,relro
    -Wl,-z,now
    -Wl,-z,noexecstack
    $<$<CONFIG:Release>:-s>
)

target_compile_definitions(mc2_server PRIVATE
    -DAICPU_COMPILE
)

target_link_libraries(mc2_server PRIVATE
    -Wl,--no-as-needed
)