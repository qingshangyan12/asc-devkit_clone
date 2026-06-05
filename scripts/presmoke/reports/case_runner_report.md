# Presmoke Case Runner Generation Report

## Summary

- README.md count: `346`
- README with 编译运行: `269`
- generated runners: `269`
- target runnable on dav-2201/npu: `171`
- CMakeLists.txt directories: `280`
- low/medium confidence runners: `143`
- supported arch counts: `{'dav-2002': 2, 'dav-2201': 173, 'dav-3510': 253}`
- supported mode counts: `{'cpu': 190, 'npu': 266, 'sim': 219}`

## Low Confidence Runners

| case | confidence | reasons | runner |
|---|---|---|---|
| 01_simd_cpp_api/00_introduction/02_matrix/matmul_tensor_api | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/00_introduction/02_matrix/matmul_tensor_api/run.sh |
| 01_simd_cpp_api/00_introduction/03_fusion_operation/gather_adds_simt_simd_hybrid | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/00_introduction/03_fusion_operation/gather_adds_simt_simd_hybrid/run.sh |
| 01_simd_cpp_api/00_introduction/04_vector_reg/add | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/00_introduction/04_vector_reg/add/run.sh |
| 01_simd_cpp_api/00_introduction/04_vector_reg/gelu | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/00_introduction/04_vector_reg/gelu/run.sh |
| 01_simd_cpp_api/01_utilities/00_printf/simd_vf_printf | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/01_utilities/00_printf/simd_vf_printf/run.sh |
| 01_simd_cpp_api/01_utilities/02_dump/simd_vf_dump | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/01_utilities/02_dump/simd_vf_dump/run.sh |
| 01_simd_cpp_api/01_utilities/03_cpudebug | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/01_utilities/03_cpudebug/run.sh |
| 01_simd_cpp_api/01_utilities/05_sanitizer/ms_sanitizer | medium | no_run_step | scripts/presmoke/cases/01_simd_cpp_api/01_utilities/05_sanitizer/ms_sanitizer/run.sh |
| 01_simd_cpp_api/01_utilities/06_clock | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/01_utilities/06_clock/run.sh |
| 01_simd_cpp_api/01_utilities/07_simulator | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/01_utilities/07_simulator/run.sh |
| 01_simd_cpp_api/02_features/00_compilation/custom_op | low | special_02_features_00_01_02, custom_op_package_provider | scripts/presmoke/cases/01_simd_cpp_api/02_features/00_compilation/custom_op/run.sh |
| 01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib | low | special_02_features_00_01_02 | scripts/presmoke/cases/01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib/run.sh |
| 01_simd_cpp_api/02_features/00_compilation/parallel_ops_package | low | no_make_step, special_02_features_00_01_02 | scripts/presmoke/cases/01_simd_cpp_api/02_features/00_compilation/parallel_ops_package/run.sh |
| 01_simd_cpp_api/02_features/01_invocation/aclnn_invocation | low | special_02_features_00_01_02, requires_custom_op_package | scripts/presmoke/cases/01_simd_cpp_api/02_features/01_invocation/aclnn_invocation/run.sh |
| 01_simd_cpp_api/02_features/01_invocation/aclop_invocation | low | atc_command, shell_variable_reference, special_02_features_00_01_02, requires_custom_op_package | scripts/presmoke/cases/01_simd_cpp_api/02_features/01_invocation/aclop_invocation/run.sh |
| 01_simd_cpp_api/02_features/02_framework/00_pytorch/pybind | low | special_02_features_00_01_02 | scripts/presmoke/cases/01_simd_cpp_api/02_features/02_framework/00_pytorch/pybind/run.sh |
| 01_simd_cpp_api/02_features/02_framework/00_pytorch/torch_library | low | special_02_features_00_01_02 | scripts/presmoke/cases/01_simd_cpp_api/02_features/02_framework/00_pytorch/torch_library/run.sh |
| 01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin | low | atc_command, framework_command, shell_variable_reference, special_02_features_00_01_02, requires_custom_op_package | scripts/presmoke/cases/01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/auxscalar_reg | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/auxscalar_reg/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/broadcast_ub2l0c | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/broadcast_ub2l0c/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_in_tensor_api | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_in_tensor_api/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_out_tensor_api | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_out_tensor_api/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_ub2ub | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_ub2ub/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1 | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2ub_nddma | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2ub_nddma/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_ub2l1 | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_ub2l1/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/gather_ld_reg | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/gather_ld_reg/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_align | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_align/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_mask | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_mask/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_unalign | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_unalign/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/move_reg | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/move_reg/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/scatter_st_reg | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/scatter_st_reg/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1 | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2ub | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2ub/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dmx_l12l0 | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dmx_l12l0/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dv2_l12l0 | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dv2_l12l0/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0 | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2 | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_mx | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_mx/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_tensor_api | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_tensor_api/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/cast | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/cast/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/compare | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/compare/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_compound_compute | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_compound_compute/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_logic | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_logic/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/gather | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/gather/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/interleave_pair | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/interleave_pair/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/mrg_sort | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/mrg_sort/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/region_proposal_sort | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/region_proposal_sort/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/select | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/select/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/transpose | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/transpose/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/abs | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/abs/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/arange | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/arange/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/cast | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/cast/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/compare | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/compare/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/data_relayout | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/data_relayout/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/duplicate | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/duplicate/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/gather | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/gather/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/histogram | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/histogram/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mergemode | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mergemode/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mul | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mul/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/muls | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/muls/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_block | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_block/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_pair | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_pair/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/select | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/select/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/squeeze | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/squeeze/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/subnormal | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/subnormal/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/trait | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/trait/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/truncate | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/truncate/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/04_resource_management/get_ub_size | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/04_resource_management/get_ub_size/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tbufpool_management | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tbufpool_management/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/cross_core_set_wait_flag | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/06_sync_control/cross_core_set_wait_flag/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/mutex | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/06_sync_control/mutex/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/reg_sync | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/06_sync_control/reg_sync/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/07_system_access/ctrl_spr | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/07_system_access/ctrl_spr/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/data_movement_with_atomic_operations | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/data_movement_with_atomic_operations/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/scalar_atomic_operations | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/scalar_atomic_operations/run.sh |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation/run.sh |
| 01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming | medium | no_cmake_step, no_make_step, requires_custom_op_package | scripts/presmoke/cases/01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_format_column_major | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_format_column_major/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8 | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareAB | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareAB/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareB | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareB/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_mx | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_mx/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_scale_cache | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_scale_cache/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_ub_tscm_nz | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_ub_tscm_nz/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_splitm | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_splitm/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_tscm_src_vecout | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_tscm_src_vecout/run.sh |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_vecout | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/00_matrix/matmul_vecout/run.sh |
| 01_simd_cpp_api/03_libraries/08_tensor_transformation/broadcast | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/08_tensor_transformation/broadcast/run.sh |
| 01_simd_cpp_api/03_libraries/08_tensor_transformation/transdata | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/08_tensor_transformation/transdata/run.sh |
| 01_simd_cpp_api/03_libraries/11_random/philoxrandom | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/11_random/philoxrandom/run.sh |
| 01_simd_cpp_api/03_libraries/12_math/bitwiseand | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/12_math/bitwiseand/run.sh |
| 01_simd_cpp_api/03_libraries/12_math/clamp | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/12_math/clamp/run.sh |
| 01_simd_cpp_api/03_libraries/12_math/clampmin | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/12_math/clampmin/run.sh |
| 01_simd_cpp_api/03_libraries/12_math/fma | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/12_math/fma/run.sh |
| 01_simd_cpp_api/03_libraries/12_math/isfinite | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/12_math/isfinite/run.sh |
| 01_simd_cpp_api/03_libraries/12_math/logicaland | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/12_math/logicaland/run.sh |
| 01_simd_cpp_api/03_libraries/12_math/power | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/12_math/power/run.sh |
| 01_simd_cpp_api/03_libraries/12_math/rint | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/12_math/rint/run.sh |
| 01_simd_cpp_api/03_libraries/12_math/where | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/03_libraries/12_math/where/run.sh |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_basic_api_high_performance | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_basic_api_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_high_performance | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_basic_api_high_performance | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_basic_api_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_high_performance | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_tensor_api_high_performance | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_tensor_api_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_eltwise_high_performance | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_eltwise_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_high_performance | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matmul_gelu_high_performance | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matmul_gelu_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matrix_transpose | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matrix_transpose/run.sh |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance/run.sh |
| 01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy | low | shell_variable_reference | scripts/presmoke/cases/01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy/run.sh |
| 02_simd_c_api/00_introduction/04_reg_base_add_compute/c_api_simd_add | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/02_simd_c_api/00_introduction/04_reg_base_add_compute/c_api_simd_add/run.sh |
| 03_simt_api/00_introduction/00_quickstart/hello_world_simt | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/00_introduction/00_quickstart/hello_world_simt/run.sh |
| 03_simt_api/00_introduction/01_gather/basic_gather/gather_1d | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/00_introduction/01_gather/basic_gather/gather_1d/run.sh |
| 03_simt_api/00_introduction/01_gather/basic_gather/gather_2d | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/00_introduction/01_gather/basic_gather/gather_2d/run.sh |
| 03_simt_api/00_introduction/01_gather/general_gather | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/00_introduction/01_gather/general_gather/run.sh |
| 03_simt_api/01_utilities/00_printf | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/01_utilities/00_printf/run.sh |
| 03_simt_api/01_utilities/01_assert | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/01_utilities/01_assert/run.sh |
| 03_simt_api/01_utilities/02_clock | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/01_utilities/02_clock/run.sh |
| 03_simt_api/01_utilities/03_profiling/msProf | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/01_utilities/03_profiling/msProf/run.sh |
| 03_simt_api/01_utilities/07_simulator | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/01_utilities/07_simulator/run.sh |
| 03_simt_api/01_utilities/09_log | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/01_utilities/09_log/run.sh |
| 03_simt_api/02_features/00_framework/00_pytorch/torch_library_roll | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/02_features/00_framework/00_pytorch/torch_library_roll/run.sh |
| 03_simt_api/02_features/01_api_features/00_memory_asscess/insert_hash_table | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/02_features/01_api_features/00_memory_asscess/insert_hash_table/run.sh |
| 03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence/run.sh |
| 03_simt_api/02_features/01_api_features/02_atomic_operation/histogram | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/02_features/01_api_features/02_atomic_operation/histogram/run.sh |
| 03_simt_api/02_features/06_compile/00_basic_compile | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/02_features/06_compile/00_basic_compile/run.sh |
| 03_simt_api/02_features/06_compile/01_separate_compile | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/02_features/06_compile/01_separate_compile/run.sh |
| 03_simt_api/02_features/06_compile/02_dynamic_library_compile | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/02_features/06_compile/02_dynamic_library_compile/run.sh |
| 03_simt_api/02_features/06_compile/03_static_library_compile | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/02_features/06_compile/03_static_library_compile/run.sh |
| 03_simt_api/03_best_practices/00_memory_optimizations/aligned_types | medium | unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/03_best_practices/00_memory_optimizations/aligned_types/run.sh |
| 03_simt_api/03_best_practices/00_memory_optimizations/cache_hint | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/run.sh |
| 03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice/run.sh |
| 03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add | low | shell_variable_reference, unsupported_on_target_arch_or_mode | scripts/presmoke/cases/03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add/run.sh |

## CMake Directories Without README

- `01_simd_cpp_api/02_features/00_compilation/custom_op/framework`
- `01_simd_cpp_api/02_features/00_compilation/custom_op/framework/onnx_plugin`
- `01_simd_cpp_api/02_features/00_compilation/custom_op/framework/tf_plugin`
- `01_simd_cpp_api/02_features/00_compilation/custom_op/op_host`
- `01_simd_cpp_api/02_features/00_compilation/custom_op/op_kernel`
- `01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib/app`
- `01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib/op`
- `01_simd_cpp_api/02_features/00_compilation/parallel_ops_package/add_custom`
- `01_simd_cpp_api/02_features/00_compilation/parallel_ops_package/leaky_relu_custom`
- `01_simd_cpp_api/02_features/00_compilation/parallel_ops_package/leaky_relu_custom/framework`
- `01_simd_cpp_api/02_features/00_compilation/parallel_ops_package/leaky_relu_custom/framework/onnx_plugin`
- `01_simd_cpp_api/02_features/00_compilation/parallel_ops_package/leaky_relu_custom/op_host`
- `01_simd_cpp_api/02_features/00_compilation/parallel_ops_package/leaky_relu_custom/op_kernel`

## README Without 编译运行

- `.`
- `01_simd_cpp_api`
- `01_simd_cpp_api/00_introduction`
- `01_simd_cpp_api/00_introduction/00_quickstart`
- `01_simd_cpp_api/00_introduction/01_vector`
- `01_simd_cpp_api/00_introduction/02_matrix`
- `01_simd_cpp_api/00_introduction/03_fusion_operation`
- `01_simd_cpp_api/00_introduction/04_vector_reg`
- `01_simd_cpp_api/01_utilities`
- `01_simd_cpp_api/01_utilities/00_printf`
- `01_simd_cpp_api/01_utilities/02_dump`
- `01_simd_cpp_api/01_utilities/04_profiling`
- `01_simd_cpp_api/01_utilities/05_sanitizer`
- `01_simd_cpp_api/02_features`
- `01_simd_cpp_api/02_features/00_compilation`
- `01_simd_cpp_api/02_features/01_invocation`
- `01_simd_cpp_api/02_features/02_framework`
- `01_simd_cpp_api/02_features/02_framework/00_pytorch`
- `01_simd_cpp_api/02_features/02_framework/01_tensorflow`
- `01_simd_cpp_api/02_features/02_framework/02_onnx`
- `01_simd_cpp_api/02_features/03_basic_api`
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement`
- `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute`
- `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute`
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute`
- `01_simd_cpp_api/02_features/03_basic_api/04_resource_management`
- `01_simd_cpp_api/02_features/03_basic_api/06_sync_control`
- `01_simd_cpp_api/02_features/03_basic_api/07_system_access`
- `01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations`
- `01_simd_cpp_api/02_features/03_basic_api/09_scalar_computation`
- `01_simd_cpp_api/02_features/04_aicpu`
- `01_simd_cpp_api/02_features/05_aclrtc`
- `01_simd_cpp_api/03_libraries`
- `01_simd_cpp_api/03_libraries/00_matrix`
- `01_simd_cpp_api/03_libraries/01_activation`
- `01_simd_cpp_api/03_libraries/02_normalization`
- `01_simd_cpp_api/03_libraries/03_quantization`
- `01_simd_cpp_api/03_libraries/04_reduce`
- `01_simd_cpp_api/03_libraries/05_sort`
- `01_simd_cpp_api/03_libraries/06_index`
- `01_simd_cpp_api/03_libraries/07_filter`
- `01_simd_cpp_api/03_libraries/08_tensor_transformation`
- `01_simd_cpp_api/03_libraries/11_random`
- `01_simd_cpp_api/03_libraries/12_math`
- `01_simd_cpp_api/03_libraries/13_utils`
- `01_simd_cpp_api/04_best_practices`
- `01_simd_cpp_api/04_best_practices/00_vector_compute_practices`
- `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices`
- `01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices`
- `01_simd_cpp_api/04_best_practices/03_fusion_compute_practices`
- `01_simd_cpp_api/04_best_practices/04_memory_access_practices`
- `01_simd_cpp_api/05_compatibility_guide`
- `02_simd_c_api`
- `02_simd_c_api/00_introduction`
- `02_simd_c_api/00_introduction/00_quickstart`
- `02_simd_c_api/00_introduction/01_add`
- `02_simd_c_api/00_introduction/04_reg_base_add_compute`
- `02_simd_c_api/01_utilities`
- `02_simd_c_api/02_features`
- `02_simd_c_api/02_features/03_c_api`
- `02_simd_c_api/02_features/03_c_api/00_vector_compute`
- `03_simt_api`
- `03_simt_api/00_introduction`
- `03_simt_api/00_introduction/00_quickstart`
- `03_simt_api/00_introduction/01_gather`
- `03_simt_api/00_introduction/01_gather/basic_gather`
- `03_simt_api/01_utilities`
- `03_simt_api/01_utilities/03_profiling`
- `03_simt_api/02_features`
- `03_simt_api/02_features/00_framework`
- `03_simt_api/02_features/00_framework/00_pytorch`
- `03_simt_api/02_features/01_api_features`
- `03_simt_api/02_features/01_api_features/00_memory_asscess`
- `03_simt_api/02_features/01_api_features/01_sync_instruction`
- `03_simt_api/02_features/06_compile`
- `03_simt_api/03_best_practices`
- `03_simt_api/03_best_practices/00_memory_optimizations`

## Planner Suggestions

- `01_simd_cpp_api/00_introduction/02_matrix/matmul_tensor_api` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/00_introduction/03_fusion_operation/gather_adds_simt_simd_hybrid` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/00_introduction/04_vector_reg/add` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/00_introduction/04_vector_reg/gelu` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/01_utilities/00_printf/simd_vf_printf` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/01_utilities/02_dump/simd_vf_dump` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/01_utilities/03_cpudebug` info: Excluded on unsupported mode npu Supported modes: cpu
- `01_simd_cpp_api/01_utilities/06_clock` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/01_utilities/07_simulator` info: Excluded on unsupported mode npu Supported modes: sim
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/auxscalar_reg` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/broadcast_ub2l0c` info: Excluded on unsupported arch dav-2201 Supported archs: dav-2002
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_in_tensor_api` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_out_tensor_api` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2ub_nddma` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_ub2l1` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/gather_ld_reg` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_align` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_mask` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/ld_st_reg_unalign` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/move_reg` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/scatter_st_reg` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2ub` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dmx_l12l0` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dv2_l12l0` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_mx` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_tensor_api` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/interleave_pair` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/region_proposal_sort` info: Excluded on unsupported arch dav-2201 Supported archs: dav-2002
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/abs` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/arange` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/cast` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/compare` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/data_relayout` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/duplicate` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/gather` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/histogram` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mergemode` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/mul` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/muls` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_block` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_pair` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/select` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/squeeze` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/subnormal` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/trait` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/truncate` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/04_resource_management/get_ub_size` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/mutex` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/reg_sync` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/07_system_access/ctrl_spr` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/scalar_atomic_operations` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/00_matrix/matmul_format_column_major` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/00_matrix/matmul_mx` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_scale_cache` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_ub_tscm_nz` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/00_matrix/matmul_splitm` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/00_matrix/matmul_tscm_src_vecout` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/00_matrix/matmul_vecout` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/11_random/philoxrandom` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/12_math/bitwiseand` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/12_math/clamp` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/12_math/fma` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/12_math/isfinite` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/12_math/logicaland` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/12_math/rint` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/03_libraries/12_math/where` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_basic_api_high_performance` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_high_performance` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_tensor_api_high_performance` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_eltwise_high_performance` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/gelu_high_performance` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matrix_transpose` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_and_simd_high_performance` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `02_simd_c_api/00_introduction/04_reg_base_add_compute/c_api_simd_add` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/00_introduction/00_quickstart/hello_world_simt` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/00_introduction/01_gather/basic_gather/gather_1d` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/00_introduction/01_gather/basic_gather/gather_2d` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/00_introduction/01_gather/general_gather` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/01_utilities/00_printf` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/01_utilities/01_assert` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/01_utilities/02_clock` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/01_utilities/03_profiling/msProf` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/01_utilities/07_simulator` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/01_utilities/09_log` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/02_features/00_framework/00_pytorch/torch_library_roll` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/02_features/01_api_features/00_memory_asscess/insert_hash_table` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/02_features/01_api_features/02_atomic_operation/histogram` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/02_features/06_compile/00_basic_compile` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/02_features/06_compile/01_separate_compile` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/02_features/06_compile/02_dynamic_library_compile` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/02_features/06_compile/03_static_library_compile` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/03_best_practices/00_memory_optimizations/aligned_types` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/03_best_practices/00_memory_optimizations/cache_hint` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
- `03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add` info: Excluded on unsupported arch dav-2201 Supported archs: dav-3510
