# Presmoke Suggestions

| example | severity | category | message | hint |
|---|---|---|---|---|
| 01_simd_cpp_api/00_introduction/02_matrix/matmul_tensor_api | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/00_introduction/03_fusion_operation/gather_adds_simt_simd_hybrid | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/00_introduction/04_vector_reg/add | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/00_introduction/04_vector_reg/gelu | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/01_utilities/00_printf/simd_vf_printf | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/01_utilities/02_dump/simd_vf_dump | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/01_utilities/03_cpudebug | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/01_utilities/03_cpudebug | info | case-runner | Skipped on unsupported mode npu | Supported modes: cpu |
| 01_simd_cpp_api/01_utilities/05_sanitizer/ms_sanitizer | info | case-runner | Generated runner confidence is medium | no_run_step |
| 01_simd_cpp_api/01_utilities/05_sanitizer/ms_sanitizer | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/01_utilities/06_clock | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/01_utilities/07_simulator | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/01_utilities/07_simulator | info | case-runner | Skipped on unsupported mode npu | Supported modes: sim |
| 01_simd_cpp_api/02_features/00_compilation/custom_op | warn | case-runner | Generated runner confidence is low | special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/00_compilation/custom_op | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib | warn | case-runner | Generated runner confidence is low | special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/00_compilation/parallel_ops_package | warn | case-runner | Generated runner confidence is low | no_make_step, special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/00_compilation/parallel_ops_package | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/01_invocation/aclnn_invocation | warn | case-runner | Generated runner confidence is low | special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/01_invocation/aclnn_invocation | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/01_invocation/aclop_invocation | warn | case-runner | Generated runner confidence is low | atc_command, shell_variable_reference, special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/01_invocation/aclop_invocation | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/02_framework/00_pytorch/pybind | warn | case-runner | Generated runner confidence is low | special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/02_framework/00_pytorch/torch_library | warn | case-runner | Generated runner confidence is low | special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_builtin | warn | case-runner | Generated runner confidence is low | no_cmake_step, no_make_step, special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_builtin | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom | warn | case-runner | Generated runner confidence is low | special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin | warn | case-runner | Generated runner confidence is low | atc_command, framework_command, shell_variable_reference, special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/auxscalar_reg | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/broadcast_ub2l0c | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/broadcast_ub2l0c | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2002 |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_in_tensor_api | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_out_tensor_api | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_ub2ub | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1 | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2ub_nddma | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_ub2l1 | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/gather_ld_reg | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_align | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_mask | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_unalign | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/move_reg | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/scatter_st_reg | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1 | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2ub | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dmx_l12l0 | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dv2_l12l0 | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0 | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0 | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2 | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_mx | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_tensor_api | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_with_sparse | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/cast | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/compare | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_compound_compute | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_logic | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/gather | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/interleave_pair | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/mrg_sort | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/region_proposal_sort | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/region_proposal_sort | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2002 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/select | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/transpose | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/abs | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/arange | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/cast | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/compare | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/data_relayout | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/duplicate | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/gather | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/histogram | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mergemode | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mul | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/muls | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_block | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_pair | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/select | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/squeeze | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/subnormal | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/trait | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/truncate | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/04_resource_management/get_ub_size | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tbufpool_management | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/cross_core_set_wait_flag | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/mutex | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/reg_sync | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/07_system_access/ctrl_spr | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/data_movement_with_atomic_operations | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/scalar_atomic_operations | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming | info | case-runner | Generated runner confidence is medium | no_cmake_step, no_make_step |
| 01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_co1_output | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_format_column_major | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareAB | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareB | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_int4 | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_mx | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_scale_cache | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_ub_tscm_nz | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_sparse | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_splitm | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_tscm_src_vecout | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_vecout | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/08_tensor_transformation/broadcast | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/08_tensor_transformation/transdata | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/11_random/philoxrandom | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/12_math/bitwiseand | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/12_math/clamp | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/12_math/clampmin | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/12_math/fma | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/12_math/isfinite | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/12_math/logicaland | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/12_math/power | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/12_math/rint | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/03_libraries/12_math/where | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_basic_api_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_basic_api_high_performance | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_tensor_api_high_performance | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_eltwise_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matmul_gelu_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matrix_transpose | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 02_simd_c_api/00_introduction/01_add/c_api_async_add | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 02_simd_c_api/00_introduction/01_add/c_api_delicacy_async_add | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 02_simd_c_api/00_introduction/01_add/c_api_sync_add | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 02_simd_c_api/00_introduction/04_reg_base_add_compute/c_api_simd_add | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 02_simd_c_api/02_features/03_c_api/00_vector_compute/00_sync_add | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 02_simd_c_api/02_features/03_c_api/00_vector_compute/01_async_add | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 02_simd_c_api/02_features/03_c_api/00_vector_compute/02_c_api_delicacy_async_add | info | case-runner | Skipped on unsupported arch dav-3510 | Supported archs: dav-2201 |
| 03_simt_api/00_introduction/00_quickstart/hello_world_simt | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/00_introduction/01_gather/basic_gather/gather_1d | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/00_introduction/01_gather/basic_gather/gather_2d | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/00_introduction/01_gather/general_gather | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/01_utilities/00_printf | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/01_utilities/01_assert | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/01_utilities/02_clock | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/01_utilities/03_profiling/msProf | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/01_utilities/07_simulator | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/01_utilities/07_simulator | info | case-runner | Skipped on unsupported mode npu | Supported modes: sim |
| 03_simt_api/01_utilities/09_log | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/02_features/00_framework/00_pytorch/torch_library_roll | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/02_features/01_api_features/00_memory_asscess/insert_hash_table | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 03_simt_api/02_features/01_api_features/02_atomic_operation/histogram | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/02_features/06_compile/00_basic_compile | warn | case-runner | Generated runner confidence is low | no_cmake_step, no_make_step, shell_variable_reference, unsupported_on_target_arch_or_mode |
| 03_simt_api/02_features/06_compile/01_separate_compile | warn | case-runner | Generated runner confidence is low | no_cmake_step, no_make_step, shell_variable_reference, unsupported_on_target_arch_or_mode |
| 03_simt_api/02_features/06_compile/02_dynamic_library_compile | warn | case-runner | Generated runner confidence is low | line_continuation, no_cmake_step, no_make_step, shell_variable_reference, unsupported_on_target_arch_or_mode |
| 03_simt_api/02_features/06_compile/03_static_library_compile | warn | case-runner | Generated runner confidence is low | line_continuation, no_cmake_step, no_make_step, shell_variable_reference, unsupported_on_target_arch_or_mode |
| 03_simt_api/03_best_practices/00_memory_optimizations/aligned_types | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 03_simt_api/03_best_practices/00_memory_optimizations/cache_hint | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
