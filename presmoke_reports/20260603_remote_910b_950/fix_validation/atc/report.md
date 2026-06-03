# Presmoke Report

- host_arch: `dav-2201`
- modes: `npu`
- started_at: `2026-06-03T14:13:40+08:00`
- finished_at: `2026-06-03T14:14:18+08:00`
- pass/fail/skip: `1/1/0`

| example | arch | mode | status | rc | duration_s | npu_wait_s | reason | steps | log |
|---|---|---|---:|---:|---:|---:|---|---|---|
| 01_simd_cpp_api/02_features/01_invocation/aclop_invocation | dav-2201 | npu | FAIL | 255 | 10.8 | 0.0 | nonzero rc | build:10.8s/rc=255 | /data/zgx/asc-devkit/presmoke_results/20260603_atc_after_soc_fix/logs/01_simd_cpp_api__02_features__01_invocation__aclop_invocation__npu.log |
| 01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin | dav-2201 | npu | PASS | 0 | 38.5 | 0.0 |  | build:33.3s<br>run:5.1s/npu/wait=0.0s<br>verify:0.0s | /data/zgx/asc-devkit/presmoke_results/20260603_atc_after_soc_fix/logs/01_simd_cpp_api__02_features__02_framework__02_onnx__onnx_plugin__npu.log |

## Suggestions

| example | severity | category | message | hint |
|---|---|---|---|---|
| 01_simd_cpp_api/02_features/01_invocation/aclop_invocation | warn | case-runner | Generated runner confidence is low | atc_command, shell_variable_reference, special_02_features_00_01_02 |
| 01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin | warn | case-runner | Generated runner confidence is low | atc_command, framework_command, shell_variable_reference, special_02_features_00_01_02 |
