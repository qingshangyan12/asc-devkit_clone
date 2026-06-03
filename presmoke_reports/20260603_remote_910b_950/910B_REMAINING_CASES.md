# 910B Remaining Cases Report

## Summary

- Baseline 910B full run: 2026-06-03 10:23:58 to 10:55:53, `dav-2201`, `npu`.
- Baseline runnable cases on 910B: `172`.
- Baseline result: `PASS 127` / `FAIL 45` / `SKIP 97`.
- Runner or command-generation failures fixed and validated: `40`.
- Confirmed remaining real failures after latest validation: `1`.
- Current explicit environment skips: `2` TensorFlow cases.
- Baseline failures cleared by card 7 rerun: `2`.
- Newly classified 910B unsupported case: `1`.

## Current Remaining Status

| bucket | count | cases |
|---|---:|---|
| Confirmed failure after latest validation | 1 | `mmad_unitflag` |
| Explicit skip | 2 | `tensorflow_builtin`, `tensorflow_custom` |
| Cleared on card 7 rerun | 2 | `sync_all`, `bank_conflict_nd2nz` |
| 910B unsupported / 950 only | 1 | `matmul_fp8` |

Counting unique non-passing or explicit-skip 910B planned cases gives `3`: `mmad_unitflag`, `tensorflow_builtin`, `tensorflow_custom`. `matmul_fp8` is 950-only and is no longer planned on 910B.

## Card 7 Rerun

- Machine: `ascend910b-smoke`, container `zgx_smoke`.
- Directory: `/data/zgx/asc-devkit`.
- Card: `ASCEND_RT_VISIBLE_DEVICES=7`.
- Local copied result: `presmoke_reports/20260603_remote_910b_950/card7_three_remaining/results`.
- Remote result: `/tmp/presmoke_card7_three_remaining_20260603_151307/results`.
- Result: `PASS 2` / `FAIL 1` / `SKIP 0`.

## Case Details

| case | current status | phase | evidence | next action |
|---|---:|---:|---|---|
| `01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_builtin` | `SKIP` | `build/run/verify` | Current runner prints `SKIP: requires TensorFlow 2.6.5 environment; skipped by presmoke`. Baseline full run failed before this explicit skip change. | Keep skipped unless a TF 2.6.5 environment is prepared. |
| `01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom` | `SKIP` | `build/run/verify` | Current runner prints `SKIP: requires TensorFlow 2.6.5 environment; skipped by presmoke`. | Keep skipped unless a TF 2.6.5 environment is prepared. |
| `01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag` | `FAIL` | `verify` | Reproduced on 910B card 7. `build=8.3s`, `run=12.0s`, `verify=0.2s`; verify prints many `rdiff: 0.000001`, then `[ERROR] result error`. | Treat as confirmed sample/result issue on 910B, not runner issue. |
| `01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all` | `PASS_ON_CARD7` | `all` | Card 7 rerun passed: total `12.2s`, build `9.2s`, run `2.9s`, verify `0.2s`; verify prints `error ratio: 0.0000` and `test pass!`. | Treat earlier baseline failure as card/runtime-state sensitive unless it reproduces. |
| `01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8` | `EXCLUDED_ON_910B_UNSUPPORTED` | `planner` | README "支持的产品" only lists `Ascend 950PR/Ascend 950DT` (910D/950, `dav-3510`). Card 7 rerun on 910B failed in build with `unknown type name 'hifloat8_t'`, matching unsupported arch behavior. | Presmoke marks this case as `dav-3510` only; 910B does not plan it and does not count it as skip. |
| `01_simd_cpp_api/04_best_practices/04_memory_access_practices/bank_conflict_nd2nz` | `PASS_ON_CARD7` | `all` | Card 7 rerun passed: total `14.1s`, build `10.5s`, run `2.9s`, verify `0.7s`; verify prints `test pass!`. | Treat earlier baseline failure as card/runtime-state sensitive unless it reproduces. |

## Already Cleared

- `aclop_invocation`: PASS after installing `custom_op` package first.
- `tiling_sink_programming`: PASS after installing `custom_op` package first.
- `quant_group_matmul_high_performance`: PASS on card 7; slow time is in build/data generation, not NPU run.
- `sync_all`: PASS on card 7 rerun.
- `bank_conflict_nd2nz`: PASS on card 7 rerun.
- The original 40 command-generation failures now pass after fixing CMake build args, README variables, ATC workdir/soc version, ONNX generation order, and custom-op package prerequisites.
