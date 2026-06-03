# 910B 失败修复回归报表

## 摘要

- 原始失败 case：`45`
- 原始判断为 runner/命令生成错误：`40`
- 这 40 个命令错误修复后：`PASS 40` / `FAIL 0`
- 命令错误中已确认 runner 修复通过：`40`
- 命令错误中剩余失败：`0`
- 补充回归：`aclop_invocation` 与 `tiling_sink_programming` 已于 2026-06-03 14:38 在 910B 卡 1 通过。
- 补充回归：`quant_group_matmul_high_performance` 已于 2026-06-03 14:42 在 910B 卡 7 通过；`mmad_unitflag` 在卡 7 复现 verify 失败。
- 补充回归：`sync_all` 与 `bank_conflict_nd2nz` 已于 2026-06-03 15:13 在 910B 卡 7 通过；`matmul_fp8` 在卡 7 为 build 失败，后按 README 支持产品确认该 case 仅支持 910D/950 (`dav-3510`)，910B 不规划、不计 skip。

## 剩余未通过/未确认

| case | 原分类 | 修复后阶段 | 修复后分类 | 证据 |
|---|---|---:|---|---|
| `01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom` | `environment_missing_dependency` | `` | `explicit_skip` | 需要 TensorFlow 2.6.5 环境，presmoke 中显式跳过。 |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag` | `runtime_or_verify_failure` | `verify` | `still_failed_other` | nonzero rc |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8` | `sample_compile_or_arch_issue` | `planner` | `excluded_unsupported_arch` | README 支持产品为 Ascend 950PR/Ascend 950DT；presmoke 已标为 `dav-3510` only，910B 不规划。 |

## 修复点

- `cmake --build` 不再追加 `-DCMAKE_ASC_ARCHITECTURES` / run-mode 参数；这些只加在 configure 命令。
- README 中的变量赋值和变量引用不再被单引号固化，例如 `SCENARIO_NUM=$SCENARIO`、`TESTCASE=$TESTCASE`。
- `atc` 按 README 的 `cd build` 语义在 build 目录执行。
- ATC `${soc_version}` 与 CMake `ARCH` 分离：910B 上 `dav-2201` 映射为 `Ascend910B4`，并支持 `PRESMOKE_SOC_VERSION` 覆盖。
- `onnx_plugin` 的 `python3 ../leaky_relu.py` 被保留在 build 阶段且排在 ATC 前面，避免先 ATC 后生成 ONNX。
- 依赖 `custom_op` 包的 case 在 build 前自动执行 `custom_op` 编译、打包、部署，并用锁避免并发安装冲突。
- TensorFlow case 需要 TensorFlow 2.6.5 环境，presmoke 中显式标记为 skip。

## 仍需处理

- `mmad_unitflag` 是 verify 阶段非零退出；卡 7 复现，多处 rdiff 约 `0.000001`，最终 `[ERROR] result error`。
- `matmul_fp8` 已按 README 支持产品标为 910B unsupported，且不再纳入 910B 规划/skip 统计；910B 剩余需要修复的真实失败只剩 `mmad_unitflag`。

## TODO

- `[confirmed-failed]` `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag`
  - 910B 上已多次复现 verify 失败，不再按 runner 问题处理。
  - 950 上该 case 通过，当前 TODO 是检查 910B 下 `unitFlag` 结果精度/容差或样例实现差异。
  - 最新复现：2026-06-03 14:42，卡 7，`/tmp/presmoke_card7_mmad_quant_20260603_144249/results`。

## 补充回归 2026-06-03 14:38

- 机器：`ascend910b-smoke` / 容器 `zgx_smoke`
- 目录：`/data/zgx/asc-devkit`
- 卡：`ASCEND_RT_VISIBLE_DEVICES=1`
- 远端结果：`/tmp/presmoke_two_cases_20260603_143858/results`
- 结果：`PASS 2` / `FAIL 0` / `SKIP 0`

| case | 状态 | 总耗时(s) | build(s) | run(s) | 说明 |
|---|---:|---:|---:|---:|---|
| `01_simd_cpp_api/02_features/01_invocation/aclop_invocation` | `PASS` | `58.9` | `51.1` | `7.8` | build 阶段先完成 `custom_op` 编译/打包/部署，随后 ATC/CMake/make 与 `execute_add_op` 通过。 |
| `01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming` | `PASS` | `54.4` | `30.2` | `24.2` | 复用已安装的 `custom_op` 包，`test_add_custom_tiling_sink.py` 通过。 |

## 补充回归 2026-06-03 14:42

- 机器：`ascend910b-smoke` / 容器 `zgx_smoke`
- 目录：`/data/zgx/asc-devkit`
- 卡：`ASCEND_RT_VISIBLE_DEVICES=7`
- 远端结果：`/tmp/presmoke_card7_mmad_quant_20260603_144249/results`
- 结果：`PASS 1` / `FAIL 1` / `SKIP 0`

| case | 状态 | 总耗时(s) | build(s) | run(s) | verify(s) | 说明 |
|---|---:|---:|---:|---:|---:|---|
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag` | `FAIL` | `20.4` | `8.3` | `12.0` | `0.2` | verify 复现失败，多处 rdiff 约 `0.000001`，最终 `[ERROR] result error`。 |
| `01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance` | `PASS` | `504.4` | `494.0` | `3.9` | `6.6` | verify 输出 `error ratio: 0.0000, tolerance: 0.0001`，`test pass!`。 |

## 补充回归 2026-06-03 15:13

- 机器：`ascend910b-smoke` / 容器 `zgx_smoke`
- 目录：`/data/zgx/asc-devkit`
- 卡：`ASCEND_RT_VISIBLE_DEVICES=7`
- 远端结果：`/tmp/presmoke_card7_three_remaining_20260603_151307/results`
- 本地结果：`presmoke_reports/20260603_remote_910b_950/card7_three_remaining/results`
- 结果：`PASS 2` / `FAIL 1` / `SKIP 0`

| case | 状态 | 总耗时(s) | build(s) | run(s) | verify(s) | 说明 |
|---|---:|---:|---:|---:|---:|---|
| `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all` | `PASS` | `12.2` | `9.2` | `2.9` | `0.2` | verify 输出 `error ratio: 0.0000`，`test pass!`。 |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8` | `EXCLUDED_ON_910B_UNSUPPORTED` | `11.8` | `11.8` | `` | `` | README 支持产品为 Ascend 950PR/Ascend 950DT；卡 7 build 失败 `unknown type name 'hifloat8_t'`，与 910B unsupported 一致；当前 910B 不规划、不计 skip。 |
| `01_simd_cpp_api/04_best_practices/04_memory_access_practices/bank_conflict_nd2nz` | `PASS` | `14.1` | `10.5` | `2.9` | `0.7` | verify 输出 `test pass!`。 |

## 明细

| case | 原分类 | 修复后状态 | 阶段 | 耗时(s) | 修复后分类 |
|---|---|---:|---:|---:|---|
| `01_simd_cpp_api/02_features/00_compilation/parallel_ops_package` | `runner_command_error:cmake_build_args` | `PASS` | `` | `39.650` | `fixed_pass` |
| `01_simd_cpp_api/02_features/01_invocation/aclop_invocation` | `runner_command_error:atc_soc_version` | `PASS` | `` | `58.900` | `fixed_pass_after_custom_op_prereq` |
| `01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_builtin` | `runner_command_error:wrong_workdir` | `PASS` | `` | `5.140` | `fixed_pass` |
| `01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom` | `environment_missing_dependency` | `SKIP` | `planner` | `` | `explicit_skip_tensorflow_2_6_5` |
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
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag` | `runtime_or_verify_failure` | `FAIL` | `verify` | `20.421` | `still_failed_other_card7` |
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
| `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all` | `runtime_or_verify_failure` | `PASS` | `` | `12.243` | `fixed_pass_on_card7` |
| `01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/data_movement_with_atomic_operations` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `34.750` | `fixed_pass` |
| `01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `34.504` | `fixed_pass` |
| `01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming` | `runner_command_error:wrong_workdir` | `PASS` | `` | `54.400` | `fixed_pass_after_custom_op_prereq` |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8` | `sample_compile_or_arch_issue` | `EXCLUDED` | `planner` | `11.840` | `excluded_unsupported_arch` |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareAB` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `31.487` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareB` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `31.335` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/08_tensor_transformation/broadcast` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `18.632` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/08_tensor_transformation/transdata` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `19.489` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/12_math/clampmin` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `19.286` | `fixed_pass` |
| `01_simd_cpp_api/03_libraries/12_math/power` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `29.096` | `fixed_pass` |
| `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_basic_api_high_performance` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `35.214` | `fixed_pass` |
| `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_high_performance` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `51.937` | `fixed_pass` |
| `01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matmul_gelu_high_performance` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `35.235` | `fixed_pass` |
| `01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `504.416` | `fixed_pass_on_card7` |
| `01_simd_cpp_api/04_best_practices/04_memory_access_practices/bank_conflict_nd2nz` | `runtime_or_verify_failure` | `PASS` | `` | `14.052` | `fixed_pass_on_card7` |
| `01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy` | `runner_command_error:missing_readme_variable` | `PASS` | `` | `20.315` | `fixed_pass` |
