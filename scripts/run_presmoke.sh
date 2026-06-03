#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# 安全选项：遇到错误立即退出，使用未定义变量报错，管道失败时退出
set -euo pipefail

# 获取脚本所在目录的绝对路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# 环境配置
ASCEND_HOME_DIR="${ASCEND_HOME_DIR:-/usr/local/Ascend/ascend-toolkit/latest}"

# 加载 Ascend 环境
if [ ! -f "${ASCEND_HOME_DIR}/set_env.sh" ]; then
    echo "Error: Ascend environment file not found: ${ASCEND_HOME_DIR}/set_env.sh"
    exit 1
fi
source "${ASCEND_HOME_DIR}/set_env.sh"

# 配置路径
CODE_PATH="${PROJECT_ROOT}/examples"
LOG_PATH="${PROJECT_ROOT}/tmplog_devkit"
NPU_POOL_SIZE="${NPU_POOL_SIZE:-1}"
NPU_POOL_DIR="${PROJECT_ROOT}/tmp_npu_pool"

# 验证路径
if [ ! -d "${CODE_PATH}" ]; then
    echo "Error: Examples directory not found: ${CODE_PATH}"
    exit 1
fi

if ! [[ "${NPU_POOL_SIZE}" =~ ^[1-9][0-9]*$ ]]; then
    echo "Error: NPU_POOL_SIZE must be a positive integer: ${NPU_POOL_SIZE}"
    exit 1
fi

init_npu_pool() {
    rm -rf "${NPU_POOL_DIR}"
    mkdir -p "${NPU_POOL_DIR}"
    mkfifo "${NPU_POOL_DIR}/tokens"
    exec 9<>"${NPU_POOL_DIR}/tokens"
    rm -f "${NPU_POOL_DIR}/tokens"

    for ((i = 0; i < NPU_POOL_SIZE; i++)); do
        printf '.' >&9
    done
}

cleanup_npu_pool() {
    exec 9>&- || true
    exec 9<&- || true
    rm -rf "${NPU_POOL_DIR}"
}

run_with_npu_slot() {
    local token

    IFS= read -r -n 1 -u 9 token
    "$@"
    local rc=$?
    printf '.' >&9
    return "${rc}"
}

# 示例列表
EXAMPLE_LIST=(
    devkit_dir_check
    01_simd_cpp_api/00_introduction/00_quickstart/hello_world
    01_simd_cpp_api/00_introduction/01_vector/add
    01_simd_cpp_api/00_introduction/01_vector/add_tpipe_tque
    01_simd_cpp_api/00_introduction/02_matrix/matmul_advanced_api
    01_simd_cpp_api/00_introduction/02_matrix/matmul_basic_api
    01_simd_cpp_api/00_introduction/02_matrix/matmul_tensor_api
    01_simd_cpp_api/00_introduction/03_fusion_operation/gather_adds_simt_simd_hybrid
    01_simd_cpp_api/00_introduction/03_fusion_operation/matmul_leakyrelu_advanced_api
    01_simd_cpp_api/00_introduction/03_fusion_operation/matmul_leakyrelu_basic_api
    01_simd_cpp_api/00_introduction/04_vector_reg/add
    01_simd_cpp_api/00_introduction/04_vector_reg/gelu
    01_simd_cpp_api/01_utilities/00_printf/simd_vf_printf
    01_simd_cpp_api/01_utilities/00_printf/simple_printf
    01_simd_cpp_api/01_utilities/01_assert
    01_simd_cpp_api/01_utilities/02_dump/simd_vf_dump
    01_simd_cpp_api/01_utilities/02_dump/simple_dump
    01_simd_cpp_api/01_utilities/03_cpudebug
    01_simd_cpp_api/01_utilities/04_profiling/msprof
    01_simd_cpp_api/01_utilities/04_profiling/torch_library_report_tensor
    01_simd_cpp_api/01_utilities/05_sanitizer/ms_sanitizer
    01_simd_cpp_api/01_utilities/06_clock
    01_simd_cpp_api/01_utilities/07_simulator
    01_simd_cpp_api/02_features/00_compilation/custom_op
    01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib
    01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib/app
    01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib/op
    01_simd_cpp_api/02_features/00_compilation/parallel_ops_package
    01_simd_cpp_api/02_features/00_compilation/parallel_ops_package/add_custom
    01_simd_cpp_api/02_features/00_compilation/parallel_ops_package/leaky_relu_custom
    01_simd_cpp_api/02_features/01_invocation/aclnn_invocation
    01_simd_cpp_api/02_features/01_invocation/aclop_invocation
    01_simd_cpp_api/02_features/02_framework/00_pytorch/pybind
    01_simd_cpp_api/02_features/02_framework/00_pytorch/torch_library
    01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom
    01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/auxscalar_reg
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/broadcast_ub2l0c
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_in_tensor_api
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_out_tensor_api
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_ub2ub
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2ub_nddma
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2ub_slice
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_ub2l1
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/gather_ld_reg
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_align
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_mask
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_unalign
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/move_reg
    01_simd_cpp_api/02_features/03_basic_api/00_data_movement/scatter_st_reg
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/batch_matmul
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2ub
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dmx_l12l0
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dv2_l12l0
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_gemv
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_mx
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_tensor_api
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag
    01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_with_sparse
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/brcb
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/cast
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/compare
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/create_vec_index
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/duplicate
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_arithmetic
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_compound_compute
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_logic
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/gather
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/interleave_pair
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/mrg_sort
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/pair_reduce_sum
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/region_proposal_sort
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/select
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/transpose
    01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/abs
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/arange
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/cast
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/compare
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/data_relayout
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/duplicate
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/gather
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/histogram
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mergemode
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mul
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/muls
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_block
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_pair
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/select
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/squeeze
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/subnormal
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/trait
    01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/truncate
    01_simd_cpp_api/02_features/03_basic_api/04_resource_management/get_tpipe_ptr
    01_simd_cpp_api/02_features/03_basic_api/04_resource_management/get_ub_size
    01_simd_cpp_api/02_features/03_basic_api/04_resource_management/list_tensor_desc_input
    01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tbufpool_management
    01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tmp_buffer
    01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tpipe_reuse
    01_simd_cpp_api/02_features/03_basic_api/06_sync_control/cross_core_set_wait_flag
    01_simd_cpp_api/02_features/03_basic_api/06_sync_control/data_sync_barrier
    01_simd_cpp_api/02_features/03_basic_api/06_sync_control/group_barrier
    01_simd_cpp_api/02_features/03_basic_api/06_sync_control/ib_set_wait
    01_simd_cpp_api/02_features/03_basic_api/06_sync_control/mutex
    01_simd_cpp_api/02_features/03_basic_api/06_sync_control/reg_sync
    01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sequential_block_sync
    01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all
    01_simd_cpp_api/02_features/03_basic_api/06_sync_control/task_sync
    01_simd_cpp_api/02_features/03_basic_api/07_system_access/ctrl_spr
    01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/data_movement_with_atomic_operations
    01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/scalar_atomic_operations
    01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation
    01_simd_cpp_api/02_features/03_basic_api/09_scalar_computation/gm_by_pass_dcache
    01_simd_cpp_api/02_features/04_aicpu/aicpu_device_tiling
    01_simd_cpp_api/02_features/05_aclrtc/rtc_hello_world
    01_simd_cpp_api/02_features/05_aclrtc/rtc_template_add
    01_simd_cpp_api/03_libraries/00_matrix/batch_matmul
    01_simd_cpp_api/03_libraries/00_matrix/batch_matmul_bias_reuse
    01_simd_cpp_api/03_libraries/00_matrix/batch_matmul_iterate_n_batch
    01_simd_cpp_api/03_libraries/00_matrix/matmul
    01_simd_cpp_api/03_libraries/00_matrix/matmul_a2b2_share
    01_simd_cpp_api/03_libraries/00_matrix/matmul_async_iterate
    01_simd_cpp_api/03_libraries/00_matrix/matmul_async_iterate_all
    01_simd_cpp_api/03_libraries/00_matrix/matmul_callback
    01_simd_cpp_api/03_libraries/00_matrix/matmul_channelsplit_output
    01_simd_cpp_api/03_libraries/00_matrix/matmul_co1_output
    01_simd_cpp_api/03_libraries/00_matrix/matmul_constant_tiling
    01_simd_cpp_api/03_libraries/00_matrix/matmul_format_column_major
    01_simd_cpp_api/03_libraries/00_matrix/matmul_format_gemv
    01_simd_cpp_api/03_libraries/00_matrix/matmul_format_nd_align
    01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8
    01_simd_cpp_api/03_libraries/00_matrix/matmul_fused
    01_simd_cpp_api/03_libraries/00_matrix/matmul_fused_mannul
    01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareAB
    01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareB
    01_simd_cpp_api/03_libraries/00_matrix/matmul_int4
    01_simd_cpp_api/03_libraries/00_matrix/matmul_k_reorder_load
    01_simd_cpp_api/03_libraries/00_matrix/matmul_l0cache
    01_simd_cpp_api/03_libraries/00_matrix/matmul_l2cache
    01_simd_cpp_api/03_libraries/00_matrix/matmul_mixdualmaster
    01_simd_cpp_api/03_libraries/00_matrix/matmul_mn_double_buffer
    01_simd_cpp_api/03_libraries/00_matrix/matmul_multi_core_unaligned
    01_simd_cpp_api/03_libraries/00_matrix/matmul_mx
    01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_scale_cache
    01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_ub_tscm_nz
    01_simd_cpp_api/03_libraries/00_matrix/matmul_nbuffer33
    01_simd_cpp_api/03_libraries/00_matrix/matmul_partial_output
    01_simd_cpp_api/03_libraries/00_matrix/matmul_preload
    01_simd_cpp_api/03_libraries/00_matrix/matmul_quant
    01_simd_cpp_api/03_libraries/00_matrix/matmul_sparse
    01_simd_cpp_api/03_libraries/00_matrix/matmul_splitk
    01_simd_cpp_api/03_libraries/00_matrix/matmul_splitm
    01_simd_cpp_api/03_libraries/00_matrix/matmul_triangle
    01_simd_cpp_api/03_libraries/00_matrix/matmul_tscm
    01_simd_cpp_api/03_libraries/00_matrix/matmul_tscm_src_vecout
    01_simd_cpp_api/03_libraries/00_matrix/matmul_unitflag
    01_simd_cpp_api/03_libraries/00_matrix/matmul_vecout
    01_simd_cpp_api/03_libraries/01_activation/geglu
    01_simd_cpp_api/03_libraries/01_activation/gelu
    01_simd_cpp_api/03_libraries/01_activation/logsoftmax
    01_simd_cpp_api/03_libraries/01_activation/sigmoid
    01_simd_cpp_api/03_libraries/01_activation/softmax
    01_simd_cpp_api/03_libraries/01_activation/softmaxflashv2
    01_simd_cpp_api/03_libraries/01_activation/softmaxflashv3
    01_simd_cpp_api/03_libraries/01_activation/softmaxgrad
    01_simd_cpp_api/03_libraries/01_activation/swiglu
    01_simd_cpp_api/03_libraries/01_activation/swish
    01_simd_cpp_api/03_libraries/02_normalization/deepnorm
    01_simd_cpp_api/03_libraries/02_normalization/layernorm
    01_simd_cpp_api/03_libraries/02_normalization/layernorm_v2
    01_simd_cpp_api/03_libraries/02_normalization/rmsnorm
    01_simd_cpp_api/03_libraries/02_normalization/welford
    01_simd_cpp_api/03_libraries/03_quantization/antiquant
    01_simd_cpp_api/03_libraries/03_quantization/dequant
    01_simd_cpp_api/03_libraries/03_quantization/quant
    01_simd_cpp_api/03_libraries/04_reduce/reduce
    01_simd_cpp_api/03_libraries/04_reduce/reducemax
    01_simd_cpp_api/03_libraries/04_reduce/sum
    01_simd_cpp_api/03_libraries/05_sort/sort
    01_simd_cpp_api/03_libraries/05_sort/topk
    01_simd_cpp_api/03_libraries/06_index/arange
    01_simd_cpp_api/03_libraries/07_filter/dropout
    01_simd_cpp_api/03_libraries/07_filter/select
    01_simd_cpp_api/03_libraries/08_tensor_transformation/add_broadcast
    01_simd_cpp_api/03_libraries/08_tensor_transformation/broadcast
    01_simd_cpp_api/03_libraries/08_tensor_transformation/fill
    01_simd_cpp_api/03_libraries/08_tensor_transformation/pad
    01_simd_cpp_api/03_libraries/08_tensor_transformation/transdata
    01_simd_cpp_api/03_libraries/08_tensor_transformation/transpose
    01_simd_cpp_api/03_libraries/08_tensor_transformation/unpad
    01_simd_cpp_api/03_libraries/11_random/philoxrandom
    01_simd_cpp_api/03_libraries/12_math/acosh
    01_simd_cpp_api/03_libraries/12_math/axpy_half_float
    01_simd_cpp_api/03_libraries/12_math/bitwiseand
    01_simd_cpp_api/03_libraries/12_math/ceil
    01_simd_cpp_api/03_libraries/12_math/clamp
    01_simd_cpp_api/03_libraries/12_math/clampmin
    01_simd_cpp_api/03_libraries/12_math/cumsum
    01_simd_cpp_api/03_libraries/12_math/erf
    01_simd_cpp_api/03_libraries/12_math/exp
    01_simd_cpp_api/03_libraries/12_math/fma
    01_simd_cpp_api/03_libraries/12_math/fmod
    01_simd_cpp_api/03_libraries/12_math/frac
    01_simd_cpp_api/03_libraries/12_math/isfinite
    01_simd_cpp_api/03_libraries/12_math/log
    01_simd_cpp_api/03_libraries/12_math/logicaland
    01_simd_cpp_api/03_libraries/12_math/power
    01_simd_cpp_api/03_libraries/12_math/rint
    01_simd_cpp_api/03_libraries/12_math/where
    01_simd_cpp_api/03_libraries/12_math/xor
    01_simd_cpp_api/03_libraries/13_utils/type_traits
    01_simd_cpp_api/04_best_practices/00_vector_compute_practices/add_high_performance
    01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_basic_api_high_performance
    01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_high_performance
    01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_basic_api_high_performance
    01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_high_performance
    01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_tensor_api_high_performance
    01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_eltwise_high_performance
    01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_high_performance
    01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matmul_gelu_high_performance
    01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matrix_transpose
    01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance
    01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance
    01_simd_cpp_api/04_best_practices/04_memory_access_practices/bank_conflict_nd2nz
    01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy
    01_simd_cpp_api/05_compatibility_guide/data_copy_l1togm
    01_simd_cpp_api/05_compatibility_guide/fill
    01_simd_cpp_api/05_compatibility_guide/matmul_s4
    01_simd_cpp_api/05_compatibility_guide/pattern_transformation
    01_simd_cpp_api/05_compatibility_guide/scatter
    01_simd_cpp_api/05_compatibility_guide/set_loaddata_boundary
    02_simd_c_api/00_introduction/00_quickstart/hello_world_npu
    02_simd_c_api/00_introduction/01_add/c_api_async_add
    02_simd_c_api/00_introduction/01_add/c_api_delicacy_async_add
    02_simd_c_api/00_introduction/01_add/c_api_sync_add
    02_simd_c_api/00_introduction/04_reg_base_add_compute/c_api_simd_add
    02_simd_c_api/02_features/03_c_api/00_vector_compute/00_sync_add
    02_simd_c_api/02_features/03_c_api/00_vector_compute/01_async_add
    02_simd_c_api/02_features/03_c_api/00_vector_compute/02_c_api_delicacy_async_add
    03_simt_api/00_introduction/00_quickstart/hello_world_simt
    03_simt_api/00_introduction/01_gather/basic_gather/gather_1d
    03_simt_api/00_introduction/01_gather/basic_gather/gather_2d
    03_simt_api/00_introduction/01_gather/general_gather
    03_simt_api/01_utilities/00_printf
    03_simt_api/01_utilities/01_assert
    03_simt_api/01_utilities/03_profiling/msProf
    03_simt_api/01_utilities/07_simulator
    03_simt_api/01_utilities/09_log
    03_simt_api/02_features/01_api_features/00_memory_asscess/insert_hash_table
    03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence
    03_simt_api/02_features/01_api_features/02_atomic_operation/histogram
    03_simt_api/02_features/06_compile/00_basic_compile
    03_simt_api/02_features/06_compile/01_separate_compile
    03_simt_api/02_features/06_compile/02_dynamic_library_compile
    03_simt_api/02_features/06_compile/03_static_library_compile
    03_simt_api/03_best_practices/00_memory_optimizations/aligned_types
    03_simt_api/03_best_practices/00_memory_optimizations/cache_hint
    03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice
    03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add
)

# 时间格式化函数（秒转换为可读格式）
format_duration() {
    local seconds=$1
    if [ "${seconds}" -eq 0 ]; then
        echo "0s"
        return
    fi
    echo "${seconds}" | awk '{
        t=split("60 s 60 m 24 h 999 d", a)
        for(n=1; n<t; n+=2) {
            if($1==0) break
            s=$1%a[n] a[n+1] s
            $1=int($1/a[n])
        }
        print s
    }'
}

# devkit_dir_check执行函数
run_check() {
    local base_path=$1

    cd "${base_path}/" || return 1
    bash devkit_dir_check.sh
}

# 标准执行函数（cmake + make + gen_data + demo + verify）
run_standard() {
    local base_path=$1
    local example_name=$2

    cd "${base_path}/${example_name}/" || return 1
    rm -rf build
    mkdir -p build && cd build || return 1
    cmake .. || return 1
    make -j || return 1

    if [ -f "../scripts/gen_data.py" ]; then
        python3 ../scripts/gen_data.py || return 1
    fi

    run_with_npu_slot ./demo || return 1

    if [ -f "../scripts/verify_result.py" ]; then
        python3 ../scripts/verify_result.py output/output.bin output/golden.bin || return 1
    fi
}

# 双输出验证函数
run_two_outputs() {
    local base_path=$1
    local example_name=$2

    cd "${base_path}/${example_name}/" || return 1
    rm -rf build
    mkdir -p build && cd build || return 1
    cmake .. || return 1
    make -j || return 1

    if [ -f "../scripts/gen_data.py" ]; then
        python3 ../scripts/gen_data.py || return 1
    fi

    run_with_npu_slot ./demo || return 1

    if [ -f "../scripts/verify_result.py" ]; then
        python3 ../scripts/verify_result.py output/output1.bin output/golden1.bin output/output2.bin output/golden2.bin || return 1
    fi
}

# CPU模式执行函数
run_cpu_mode() {
    local base_path=$1
    local example_name=$2
    local executable=${3:-add}

    cd "${base_path}/${example_name}/" || return 1
    rm -rf build
    cmake -B build -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 || return 1
    cmake --build build || return 1
    run_with_npu_slot ./build/"${executable}" || return 1
}

# 参数化执行函数
run_with_params() {
    local base_path=$1
    local example_name=$2
    local cmake_params=${3:-}
    local gen_data_params=${4:-}
    local executable=${5:-demo}
    local verify_params=${6:-output/output.bin output/golden.bin}

    cd "${base_path}/${example_name}/" || return 1
    rm -rf build
    mkdir -p build && cd build || return 1
    cmake .. ${cmake_params} || return 1
    make -j || return 1

    if [ -f "../scripts/gen_data.py" ]; then
        python3 ../scripts/gen_data.py ${gen_data_params} || return 1
    fi

    run_with_npu_slot ./"${executable}" || return 1

    if [ -f "../scripts/verify_result.py" ]; then
        python3 ../scripts/verify_result.py ${verify_params} || return 1
    fi
}

run_with_params_2() {
    local base_path=$1
    local example_name=$2
    local cmake_params=${3:-}
    local executable=${4:-demo}

    cd "${base_path}/${example_name}/" || return 1
    rm -rf build
    mkdir -p build && cd build || return 1
    cmake .. ${cmake_params} || return 1
    make -j || return 1
    run_with_npu_slot ./"${executable}" || return 1
}

# C API执行函数
run_c_api() {
    local base_path=$1
    local example_name=$2

    cd "${base_path}/${example_name}/" || return 1
    rm -rf build
    mkdir -p build && cd build || return 1
    cmake .. || return 1
    make -j || return 1
    run_with_npu_slot ./c_api_add_example || return 1
}

# msProf执行函数
run_msprof() {
    local base_path=$1
    local example_name=$2

    cd "${base_path}/${example_name}/" || return 1
    rm -rf build
    mkdir -p build && cd build || return 1
    cmake .. || return 1
    make -j || return 1

    if [ -f "../scripts/gen_data.py" ]; then
        python3 ../scripts/gen_data.py || return 1
    fi

    run_with_npu_slot msprof op ./demo || return 1

    if [ -f "../scripts/verify_result.py" ]; then
        python3 ../scripts/verify_result.py output/output.bin output/golden.bin || return 1
    fi
}

# 执行测试用例
run_test_case() {
    local code_path=$1
    local example_name=$2
    local log_path=$3

    local case_name="${example_name##*/}"
    local log_name="${example_name//\//__}"
    local log_file="${log_path}/${log_name}.log"
    local start_time end_time duration elapsed

    start_time=$(date +%s)
    echo ">>>>>>>>>>>>>>>>>>>>> $(date '+%Y-%m-%d %H:%M:%S') run ${example_name} start! <<<<<<<<<<<<<<<<<<<<<"

    local test_result=0

    set +e
    # 根据用例名称选择执行函数
    case "${case_name}" in
        devkit_dir_check)
            run_check "${SCRIPT_DIR}" 2>&1 | tee "${log_file}"
            ;;
        matmul_a2b2_share)
            run_two_outputs "${code_path}" "${example_name}" 2>&1 | tee "${log_file}"
            ;;
        matmul_ibshareAB)
            run_with_params "${code_path}" "${example_name}" "" "" "demo" "output/output_ABshare.bin output/golden.bin" 2>&1 | tee "${log_file}"
            ;;
        matmul_no_ibshareAB)
            run_with_params "${code_path}" "${example_name}" "" "" "demo" "output/output_noABshare.bin output/golden.bin" 2>&1 | tee "${log_file}"
            ;;
        matmul_b8|matmul_ibshareB)
            run_with_params "${code_path}" "${example_name}" "-DDT_MODE=0" "0" 2>&1 | tee "${log_file}"
            ;;
        hello_world_npu|list_tensor_desc_input|get_tpipe_ptr|group_barrier|acosh)
            run_with_params "${code_path}" "${example_name}" "" "" "demo" "" 2>&1 | tee "${log_file}"
            ;;
        basic_api_tque_add)
            run_with_params "${code_path}" "${example_name}" "" "" "add" 2>&1 | tee "${log_file}"
            ;;
        03_cpudebug)
            run_cpu_mode "${code_path}" "${example_name}" "cpu_debug" 2>&1 | tee "${log_file}"
            ;;
        msProf)
            run_msprof "${code_path}" "${example_name}" 2>&1 | tee "${log_file}"
            ;;
        basic_api_memory_allocator_add)
            run_with_params "${code_path}" "${example_name}" "" "" "add" 2>&1 | tee "${log_file}"
            ;;
        batch_matmul)
            run_with_params "${code_path}" "${example_name}" "-DB_SIZE=4 -DM_SIZE=30 -DK_SIZE=40 -DN_SIZE=70" "-b=4 -m=30 -k=40 -n=70" 2>&1 | tee "${log_file}"
            ;;
        mmad_load3dv2)
            run_with_params "${code_path}" "${example_name}" "-DSCENARIO_NUM=4 -DM_SIZE=30 -DK_SIZE=40 -DN_SIZE=70" "-scenarioNum=4 -m=30 -k=40 -n=70" "demo" "-scenarioNum=4 output/output.bin output/golden.bin" 2>&1 | tee "${log_file}"
            ;;
        load_data_l12l0)
            run_with_params "${code_path}" "${example_name}" "-DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=12" "-scenarioNum=12" "demo" "-scenarioNum=12 output/output.bin output/golden.bin" 2>&1 | tee "${log_file}"
            ;;
        antiquant)
            run_with_params_2 "${code_path}" "${example_name}" "-DNPU_ARCH=dav-2201" "" "demo" "" 2>&1 | tee "${log_file}"
            ;;
        gelu)
            run_with_params "${code_path}" "${example_name}" "-DNPU_ARCH=dav-2201" "" "demo" "" 2>&1 | tee "${log_file}"
            ;;
        layernorm|reducemax|sort|arange|dropout|broadcast)
            run_standard "${code_path}" "${example_name}" 2>&1 | tee "${log_file}"
            ;;
        c_api_async_add|c_api_delicacy_async_add|c_api_sync_add)
            run_c_api "${code_path}" "${example_name}" 2>&1 | tee "${log_file}"
            ;;
        *)
            run_standard "${code_path}" "${example_name}" 2>&1 | tee "${log_file}"
            ;;
    esac

    test_result=${PIPESTATUS[0]}
    set -e

    end_time=$(date +%s)
    duration=$((end_time - start_time))
    elapsed=$(format_duration "${duration}")

    echo "test case ${example_name} duration: ${elapsed}"
    if [ "${test_result}" -eq 0 ]; then
        echo "test case ${example_name} result: PASS"
    else
        echo "test case ${example_name} result: FAIL (rc=${test_result})"
    fi
    echo ">>>>>>>>>>>>>>>>>>>>> $(date '+%Y-%m-%d %H:%M:%S') run ${example_name} finished! <<<<<<<<<<<<<<<<<<<<<"

    return ${test_result}
}

# 主执行流程
main() {
    echo "Current directory: ${PWD}"
    echo "Code path: ${CODE_PATH}"
    echo "Log path: ${LOG_PATH}"

    local start_time end_time total_duration total_elapsed

    start_time=$(date +%s)
    echo "=== $(date '+%Y-%m-%d %H:%M:%S') ==="

    # 准备日志目录
    mkdir -p "${LOG_PATH}"
    rm -rf "${LOG_PATH:?}"/*
    rm -f "${PROJECT_ROOT}/result_devkit.txt" "${PROJECT_ROOT}/devkit_cases.txt"
    init_npu_pool
    trap cleanup_npu_pool EXIT

    # 并行执行所有测试用例
    local pids=()
    local pid_examples=()
    local pid_logs=()

    for example_name in "${EXAMPLE_LIST[@]}"; do
        local log_name="${example_name//\//__}"
        echo "${example_name}" >> "${PROJECT_ROOT}/devkit_cases.txt"
        run_test_case "${CODE_PATH}" "${example_name}" "${LOG_PATH}" &
        pids+=("$!")
        pid_examples+=("${example_name}")
        pid_logs+=("${LOG_PATH}/${log_name}.log")
    done

    local total_failed=0
    local total_passed=0
    local failed_cases=()

    for i in "${!pids[@]}"; do
        local pid="${pids[$i]}"
        local example_name="${pid_examples[$i]}"
        local log_file="${pid_logs[$i]}"
        local rc=0

        if wait "${pid}"; then
            rc=0
        else
            rc=$?
        fi

        if [ "${rc}" -ne 0 ]; then
            echo "${example_name} fail (rc=${rc})" >> "${PROJECT_ROOT}/result_devkit.txt"
            failed_cases+=("${example_name} (rc=${rc})")
            ((total_failed++)) || true
            continue
        fi

        if [ -s "${log_file}" ]; then
            if grep -qE "check pass|初始快照已生成|test pass|passed|\[Block \(5\/6\)\]: OUTPUT = 24" "${log_file}"; then
                echo "${example_name} pass" >> "${PROJECT_ROOT}/result_devkit.txt"
                ((total_passed++)) || true
            else
                echo "${example_name} fail (no success marker)" >> "${PROJECT_ROOT}/result_devkit.txt"
                failed_cases+=("${example_name} (no success marker)")
                ((total_failed++)) || true
            fi
        else
            echo "Warning: Log file missing or empty: ${log_file}" >&2
            echo "${example_name} fail (empty log)" >> "${PROJECT_ROOT}/result_devkit.txt"
            failed_cases+=("${example_name} (empty log)")
            ((total_failed++)) || true
        fi
    done

    # 输出测试总结
    local total_count=$((total_passed + total_failed))
    echo "========================================"
    echo "Test Summary:"
    echo "  Total:  ${total_count}"
    echo "  Passed: ${total_passed}"
    echo "  Failed: ${total_failed}"
    echo "========================================"

    # 检查是否有失败的测试
    if [ ${total_failed} -gt 0 ]; then
        echo "Failed cases:"
        printf '  %s\n' "${failed_cases[@]}"
        echo "execute samples failed" >&2
        end_time=$(date +%s)
        total_duration=$((end_time - start_time))
        total_elapsed=$(format_duration "${total_duration}")
        echo "=== $(date '+%Y-%m-%d %H:%M:%S') ==="
        echo "test cases all duration: ${total_elapsed}"
        exit 1
    else
        echo "execute samples success"
    fi

    # 输出总耗时
    end_time=$(date +%s)
    total_duration=$((end_time - start_time))
    total_elapsed=$(format_duration "${total_duration}")
    echo "=== $(date '+%Y-%m-%d %H:%M:%S') ==="
    echo "test cases all duration: ${total_elapsed}"
}

# 执行主函数
main
