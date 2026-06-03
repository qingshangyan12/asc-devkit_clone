# 910B 失败修复回归报表

## 摘要

- 原始失败 case：`45`
- 原始判断为 runner/命令生成错误：`40`
- 这 40 个命令错误修复后：`PASS 37` / `FAIL 3`
- 命令错误中已确认 runner 修复通过：`37`
- 命令错误中剩余失败：`3`

## 剩余失败

| case | 原分类 | 修复后阶段 | 修复后分类 | 证据 |
|---|---|---:|---|---|
| `01_simd_cpp_api/02_features/01_invocation/aclop_invocation` | `runner_command_error:atc_soc_version` | `build` | `prerequisite_custom_op_deploy_issue` | ATC 已使用 Ascend910B4，当前失败为已部署 AddCustom 包缺 add_custom_tiling.h；README 要求先完成 custom_op 编译/打包/部署。 |
| `01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom` | `environment_missing_dependency` | `` | `not_rerun` | 未在修复回归中运行。 |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag` | `runtime_or_verify_failure` | `verify` | `still_failed_other` | nonzero rc |
| `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all` | `runtime_or_verify_failure` | `` | `not_rerun` | 未在修复回归中运行。 |
| `01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming` | `runner_command_error:wrong_workdir` | `run` | `runtime_accuracy_failure` | 已进入 torch_npu 图模式运行，当前为输出精度断言失败。 |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8` | `sample_compile_or_arch_issue` | `` | `not_rerun` | 未在修复回归中运行。 |
| `01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance` | `runner_command_error:missing_readme_variable` | `verify` | `verify_accuracy_failure` | error ratio 0.0003 > tolerance 0.0001 |
| `01_simd_cpp_api/04_best_practices/04_memory_access_practices/bank_conflict_nd2nz` | `runtime_or_verify_failure` | `` | `not_rerun` | 未在修复回归中运行。 |

## 修复点

- `cmake --build` 不再追加 `-DCMAKE_ASC_ARCHITECTURES` / run-mode 参数；这些只加在 configure 命令。
- README 中的变量赋值和变量引用不再被单引号固化，例如 `SCENARIO_NUM=$SCENARIO`、`TESTCASE=$TESTCASE`。
- `atc` 按 README 的 `cd build` 语义在 build 目录执行。
- ATC `${soc_version}` 与 CMake `ARCH` 分离：910B 上 `dav-2201` 映射为 `Ascend910B4`，并支持 `PRESMOKE_SOC_VERSION` 覆盖。
- `onnx_plugin` 的 `python3 ../leaky_relu.py` 被保留在 build 阶段且排在 ATC 前面，避免先 ATC 后生成 ONNX。

## 仍需处理

- `aclop_invocation` 需要前置 `custom_op` 编译/打包/部署产生完整 AddCustom 包；当前部署缺 `add_custom_tiling.h`。
- `tiling_sink_programming` 是 torch_npu 图模式结果精度断言失败。
- `quant_group_matmul_high_performance` 已可编译/运行，但 verify 失败：`error ratio 0.0003 > tolerance 0.0001`，且 build/run 很慢。

## 明细

| case | 原分类 | 修复后状态 | 阶段 | 耗时(s) | 修复后分类 |
|---|---|---:|---:|---:|---|
| `01_simd_cpp_api/02_features/00_compilation/parallel_ops_package` | `runner_command_error:cmake_build_args` | `PASS` | `` | `39.650` | `fixed_pass` |
| `01_simd_cpp_api/02_features/01_invocation/aclop_invocation` | `runner_command_error:atc_soc_version` | `FAIL` | `build` | `10.754` | `prerequisite_custom_op_deploy_issue` |
| `01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_builtin` | `runner_command_error:wrong_workdir` | `PASS` | `` | `5.140` | `fixed_pass` |
| `01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom` | `environment_missing_dependency` | `` | `` | `` | `not_rerun` |
| `01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin` | `runner_command_error:atc_soc_version` | `PASS` | `` | `38.481` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_ub2ub` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `11.954` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `14.280` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `14.145` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `12.156` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `12.132` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `13.794` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `18.091` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `15.013` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `17.408` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag` | `runtime_or_verify_failure` | `FAIL` | `verify` | `12.388` | `still_failed_other` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `17.251` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/cast` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `12.245` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/compare` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `13.155` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_compound_compute` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `14.146` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_logic` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `14.139` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/gather` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `13.486` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/mrg_sort` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `13.513` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `13.499` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/select` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `12.498` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/transpose` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `12.222` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `12.413` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tbufpool_management` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `12.675` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/cross_core_set_wait_flag` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `12.776` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all` | `runtime_or_verify_failure` | `` | `` | `` | `not_rerun` |
| `01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/data_movement_with_atomic_operations` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `34.750` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `34.504` | `fixed_pass` |
| `01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming` | `runner_command_error:wrong_workdir` | `FAIL` | `run` | `25.442` | `runtime_accuracy_failure` |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8` | `sample_compile_or_arch_issue` | `` | `` | `` | `not_rerun` |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareAB` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `31.487` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareB` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `31.335` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/08_tensor_transformation/broadcast` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `18.632` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/08_tensor_transformation/transdata` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `19.489` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/12_math/clampmin` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `19.286` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/12_math/power` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `29.096` | `fixed_pass` |
| `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_basic_api_high_performance` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `35.214` | `fixed_pass` |
| `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_high_performance` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `51.937` | `fixed_pass` |
| `01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matmul_gelu_high_performance` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `35.235` | `fixed_pass` |
| `01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance` | `runner_command_error:missing_readme_variable` | `FAIL` | `verify` | `1607.034` | `verify_accuracy_failure` |
| `01_simd_cpp_api/04_best_practices/04_memory_access_practices/bank_conflict_nd2nz` | `runtime_or_verify_failure` | `` | `` | `` | `not_rerun` |
| `01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `20.315` | `fixed_pass` |
