# Presmoke Suggestions

| example | severity | category | message | hint |
|---|---|---|---|---|
| 01_simd_cpp_api/02_features/00_compilation/parallel_ops_package | warn | case-runner | Generated runner confidence is low | no_make_step, special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/01_invocation/aclop_invocation | warn | case-runner | Generated runner confidence is low | atc_command, shell_variable_reference, special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_builtin | warn | case-runner | Generated runner confidence is low | no_cmake_step, no_make_step, special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin | warn | case-runner | Generated runner confidence is low | atc_command, framework_command, shell_variable_reference, special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_ub2ub | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1 | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1 | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0 | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2 | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_mx | warn | case-runner | Generated runner confidence is low | shell_variable_reference, unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_mx | info | case-runner | Skipped on unsupported arch dav-2201 | Supported archs: dav-3510 |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_tensor_api | info | case-runner | Generated runner confidence is medium | unsupported_on_target_arch_or_mode |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_tensor_api | info | case-runner | Skipped on unsupported arch dav-2201 | Supported archs: dav-3510 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/cast | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/compare | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_compound_compute | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_logic | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/gather | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/mrg_sort | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/select | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/transpose | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tbufpool_management | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/cross_core_set_wait_flag | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/data_movement_with_atomic_operations | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming | info | case-runner | Generated runner confidence is medium | no_cmake_step, no_make_step |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareAB | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareB | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/08_tensor_transformation/broadcast | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/08_tensor_transformation/transdata | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/12_math/clampmin | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/03_libraries/12_math/power | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_basic_api_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matmul_gelu_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
| 01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy | warn | case-runner | Generated runner confidence is low | shell_variable_reference |
