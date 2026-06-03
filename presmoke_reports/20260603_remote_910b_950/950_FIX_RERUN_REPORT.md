# 950 Failed Case Fix Rerun Report

## Summary

- First targeted rerun: PASS/FAIL/SKIP = `83/13/0`, window `2026-06-03T15:35:11+08:00` to `2026-06-03T15:49:01+08:00`.
- Fix rerun: PASS/FAIL/SKIP = `10/3/0`, window `2026-06-03T15:53:56+08:00` to `2026-06-03T15:56:18+08:00`.
- Fixed after runner/env changes: `10` of `13` first-run failures.
- Remaining failures: `3`.

## First Failure Categories

| category | count |
|---|---:|
| `accuracy_or_verify` | 5 |
| `missing_en_dtypes` | 3 |
| `runner_line_continuation` | 2 |
| `runner_wrong_cwd_demo` | 2 |
| `runner_wrong_cwd_msprof` | 1 |

## Changes Validated

- `torch_library_report_tensor`: runner now executes `msprof --application="python3 ../torch_library_report_tensor.py"` from `build_npu`; rerun PASS.
- SIMT compile cases: README parser now prefers the CMake execution block and handles shell line continuations; four compile cases rerun PASS.
- MXFP4 high-performance cases: installed `en_dtypes==0.0.4` in the 950 container; kept existing `ml_dtypes==0.5.4` because README `ml_dtypes==0.2.0` cannot build on Python 3.12. Three MXFP4 cases rerun PASS.

## Remaining Failures

| case | first category | duration_s | build_s | run_s | verify_s | rc | failing step |
|---|---|---:|---:|---:|---:|---:|---|
| `01_simd_cpp_api/00_introduction/04_vector_reg/add` | `accuracy_or_verify` | 21.389 | 14.159 | 7.046 | 0.183 | 1 | `bash /home/zgx/asc-devkit/scripts/presmoke/cases/01_simd_cpp_api/00_introduction/04_vector_reg/add/run.sh verify` |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1` | `accuracy_or_verify` | 34.081 | 14.524 | 19.376 | 0.180 | 1 | `bash /home/zgx/asc-devkit/scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1/run.sh verify` |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2` | `accuracy_or_verify` | 23.036 | 8.228 | 11.334 | 3.474 | 1 | `bash /home/zgx/asc-devkit/scripts/presmoke/cases/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2/run.sh verify` |

## All First Failures After Fix

| case | first category | fix status | duration_s | build_s | run_s | verify_s | wait_s |
|---|---|---:|---:|---:|---:|---:|---:|
| `01_simd_cpp_api/00_introduction/04_vector_reg/add` | `accuracy_or_verify` | FAIL | 21.389 | 14.159 | 7.046 | 0.183 | 0.000 |
| `01_simd_cpp_api/01_utilities/04_profiling/torch_library_report_tensor` | `runner_wrong_cwd_msprof` | PASS | 141.850 | 123.778 | 18.055 | 0.013 | 0.000 |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1` | `accuracy_or_verify` | FAIL | 34.081 | 14.524 | 19.376 | 0.180 | 12.525 |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dmx_l12l0` | `accuracy_or_verify` | PASS | 27.224 | 14.522 | 12.527 | 0.174 | 6.683 |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2` | `accuracy_or_verify` | FAIL | 23.036 | 8.228 | 11.334 | 3.474 | 4.284 |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_mx` | `accuracy_or_verify` | PASS | 22.122 | 15.095 | 6.845 | 0.182 | 0.000 |
| `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_basic_api_high_performance` | `missing_en_dtypes` | PASS | 35.072 | 26.720 | 6.849 | 1.503 | 0.000 |
| `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_high_performance` | `missing_en_dtypes` | PASS | 48.036 | 36.814 | 9.748 | 1.473 | 2.903 |
| `01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_mxfp4_tensor_api_high_performance` | `missing_en_dtypes` | PASS | 49.966 | 34.892 | 13.583 | 1.491 | 6.748 |
| `03_simt_api/02_features/06_compile/00_basic_compile` | `runner_wrong_cwd_demo` | PASS | 6.992 | 1.134 | 5.844 | 0.014 | 0.000 |
| `03_simt_api/02_features/06_compile/01_separate_compile` | `runner_wrong_cwd_demo` | PASS | 8.011 | 1.167 | 6.829 | 0.014 | 0.000 |
| `03_simt_api/02_features/06_compile/02_dynamic_library_compile` | `runner_line_continuation` | PASS | 20.528 | 1.830 | 18.683 | 0.014 | 11.836 |
| `03_simt_api/02_features/06_compile/03_static_library_compile` | `runner_line_continuation` | PASS | 19.070 | 1.545 | 17.511 | 0.013 | 10.664 |
