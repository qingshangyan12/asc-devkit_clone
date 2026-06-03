# Remote Presmoke Report: 910B and 950

## Summary

| machine | arch | detected | runnable | pass | fail | skip | timeout | elapsed | started | finished | remote result |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---|---|---|
| 910B | `dav-2201` | 269 | 172 | 127 | 45 | 97 | 0 | 31:55 | 2026-06-03T10:23:58+08:00 | 2026-06-03T10:55:53+08:00 | `ascend910b-smoke:/data/zgx/asc-devkit/presmoke_results/20260603_102155_910b_dav2201_npu` |
| 950 | `dav-3510` | 269 | 242 | 148 | 94 | 27 | 0 | 48:49 | 2026-06-03T10:23:50+08:00 | 2026-06-03T11:12:39+08:00 | `ascend950-smoke:/home/zgx/asc-devkit/presmoke_results/20260603_102155_950_dav3510_npu` |

## Failure Counts

| machine | reason | count |
|---|---|---:|
| 910B | nonzero rc | 45 |
| 950 | nonzero rc | 94 |

## Top Slow Cases

| machine | case | status | duration | build | run | verify | npu_wait |
|---|---|---|---:|---:|---:|---:|---:|
| 910B | `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/task_sync` | PASS | 18:28 | 0:08 | 18:20 | 0:00 | 18:17 |
| 910B | `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all` | FAIL | 18:28 | 0:09 | 18:18 | 0:00 | 0:00 |
| 910B | `01_simd_cpp_api/02_features/03_basic_api/09_scalar_computation/gm_by_pass_dcache` | PASS | 18:25 | 0:08 | 18:17 | 0:00 | 18:14 |
| 910B | `01_simd_cpp_api/02_features/04_aicpu/aicpu_device_tiling` | PASS | 18:24 | 0:08 | 18:16 | 0:00 | 18:13 |
| 910B | `01_simd_cpp_api/02_features/02_framework/00_pytorch/torch_library` | PASS | 2:31 | 2:11 | 0:20 | 0:00 | 0:05 |
| 910B | `01_simd_cpp_api/02_features/02_framework/00_pytorch/pybind` | PASS | 2:29 | 2:13 | 0:15 | 0:00 | 0:00 |
| 910B | `01_simd_cpp_api/01_utilities/04_profiling/torch_library_report_tensor` | PASS | 2:21 | 2:01 | 0:20 | 0:00 | 0:00 |
| 910B | `01_simd_cpp_api/03_libraries/13_utils/type_traits` | PASS | 1:26 | 1:20 | 0:06 | 0:00 | 0:02 |
| 910B | `01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib` | PASS | 1:03 | 0:58 | 0:05 | 0:00 | 0:00 |
| 910B | `01_simd_cpp_api/03_libraries/00_matrix/matmul_sparse` | PASS | 0:36 | 0:32 | 0:03 | 0:00 | 0:00 |
| 950 | `01_simd_cpp_api/03_libraries/00_matrix/matmul_nbuffer33` | PASS | 19:29 | 0:24 | 19:04 | 0:00 | 18:44 |
| 950 | `01_simd_cpp_api/03_libraries/00_matrix/matmul_mx_ub_tscm_nz` | PASS | 19:17 | 0:29 | 18:48 | 0:00 | 0:18 |
| 950 | `01_simd_cpp_api/03_libraries/00_matrix/matmul_partial_output` | PASS | 19:17 | 0:26 | 18:51 | 0:00 | 18:43 |
| 950 | `01_simd_cpp_api/03_libraries/00_matrix/matmul_preload` | PASS | 19:06 | 0:26 | 18:39 | 0:00 | 18:33 |
| 950 | `01_simd_cpp_api/03_libraries/13_utils/type_traits` | PASS | 5:16 | 5:09 | 0:07 | 0:00 | 0:00 |
| 950 | `01_simd_cpp_api/02_features/02_framework/00_pytorch/torch_library` | PASS | 2:56 | 2:05 | 0:52 | 0:00 | 0:26 |
| 950 | `01_simd_cpp_api/02_features/02_framework/00_pytorch/pybind` | PASS | 2:35 | 2:01 | 0:35 | 0:00 | 0:05 |
| 950 | `01_simd_cpp_api/01_utilities/04_profiling/torch_library_report_tensor` | FAIL | 2:21 | 2:02 | 0:19 | 0:00 | 0:18 |
| 950 | `01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tmp_buffer` | PASS | 1:38 | 0:16 | 1:22 | 0:00 | 1:04 |
| 950 | `01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/reduce_pair` | PASS | 1:33 | 0:15 | 1:17 | 0:00 | 0:59 |

## Files

- `ALL_CASE_TIMINGS.tsv`: every detected case with build/run/verify timings.
- `FAILURES.tsv`: failing cases only.
- `SLOWEST_25_PER_MACHINE.tsv`: slowest cases per machine.
- `910b/report.json`, `950/report.json`: raw presmoke reports.

## Spreadsheet-Friendly CSV

- `SUMMARY.csv`: compact run summary.
- `ALL_CASE_TIMINGS.csv`: all detected cases, shortened log paths, no long command field.
- `FAILURES.csv`: failing cases only.
- `SLOWEST_25_PER_MACHINE.csv`: slowest cases per machine.

## 910B Failure Diagnosis

- 910B failed cases: `45`
- likely runner/command generation errors: `40`
- not obviously command-generation errors: `5`

| category | count |
|---|---:|
| runner_command_error:missing_readme_variable | 35 |
| runtime_or_verify_failure | 3 |
| runner_command_error:atc_soc_version | 2 |
| runner_command_error:wrong_workdir | 2 |
| runner_command_error:cmake_build_args | 1 |
| environment_missing_dependency | 1 |
| sample_compile_or_arch_issue | 1 |

Main command-generation problems:

- README shell variables were not initialized or were protected by single-quoted `bash -lc`, so `$SCENARIO_NUM`, `$SCENARIO`, `$TESTCASE`, etc. reached CMake/source literally.
- Python-only cases were executed from `build_npu` although README expects the case root directory.
- `cmake --build` commands were incorrectly appended with configure-only CMake options.
- ATC cases kept `${soc_version}` empty instead of mapping current arch/device to a concrete value.

Detailed file: `910B_FAILURE_DIAGNOSIS.csv`.

## 910B 失败修复回归（2026-06-03）

- 原始 45 个失败中，40 个判断为 runner/命令生成错误。
- 修复后精确统计这 40 个：PASS 37，FAIL 3。
- 已修复的 runner 问题包括：CMake build 参数误加、README 变量引用丢失、ATC 工作目录、ATC soc_version 映射、ONNX 生成与 ATC 顺序。
- 剩余 3 个不是同一类 runner 生成错误：`aclop_invocation` 缺前置 custom_op 部署产物，`tiling_sink_programming` 运行精度失败，`quant_group_matmul_high_performance` verify 误差超阈值。
- 详细报表：`910B_FIX_VALIDATION.md` / `910B_FIX_VALIDATION.csv`。
