# asc-devkit presmoke2 910B overnight report

## Summary

- remote: `ascend910b-smoke / zgx_smoke`
- repo: `/data/zgx/asc-devkit`, git `1701d5175`
- local copy: `/Users/zhugexun/projects/asc-devkit/presmoke_reports/20260603_jobs4_npu1`
- run: `full_card1`, card `1`
- arch/mode: `dav-2201/npu`
- jobs/npu_slots: `4/1`
- timeout: `1200s`
- started/finished: `2026-06-03T01:10:22+08:00` -> `2026-06-03T02:01:20+08:00`
- elapsed: `3058s` (`51.0min`)
- total: `172`; pass/fail/skip: `130/42/0`
- timeout failures: `0`; retry: `skipped **because** no rc=124 timeout`
- suggestions: `507`

## Notes

- 本轮是新执行逻辑：CPU 编译/准备并行，NPU run step 通过单槽 semaphore 排队，`npu_slots=1`。
- 每个 case 有总耗时；每个 command 也有 step 耗时，详见 `ALL_CASE_TIMINGS.tsv` 和 `report.json`。
- `02_features/00/01/02` 已包含在全量跑里；其中一部分 README 调用方式特殊，所以失败不都代表代码本身失败。

## Failure Breakdown

- special 00/01/02 directory failures: `4`
- likely README variable expansion/placeholder issues: `23`
- make/build failures: `14`
- verify/runtime result failures: `4`
- other command failures: `1`

## All Failures

| case | rc | duration_s | failing_step |
|---|---:|---:|---|
| 01_simd_cpp_api/02_features/00_compilation/parallel_ops_package | 1 | 0.017 | cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/01_invocation/aclop_invocation | 255 | 0.501 | atc --singleop=../add_custom.json --output=. --soc_version=${soc_version} |
| 01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom | 1 | 23.976 | python3 ../run_add_custom_tf.py |
| 01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin | 255 | 0.842 | atc --model=./leaky_relu.onnx --framework=5 --soc_version=${soc_version} --output=./leaky_relu_custom --input_shape="X:8,16,1024" --input_format=ND |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_ub2ub | 2 | 6.698 | make -j |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1 | 2 | 7.069 | make -j |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm | 1 | 0.290 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm | 1 | 0.288 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm | 1 | 0.289 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1 | 1 | 0.281 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0 | 1 | 0.274 | cmake .. '-DSCENARIO_NUM=$SCENARIO' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad | 1 | 0.271 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2 | 1 | 0.023 | cmake .. '-DSCENARIO_NUM=$SCENARIO' '-DM_SIZE=$M' '-DK_SIZE=$K' '-DN_SIZE=$N' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag | 1 | 13.750 | python3 ../scripts/verify_result.py output/output.bin output/golden.bin |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum | 2 | 6.779 | make -j |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/cast | 1 | 0.278 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/compare | 1 | 0.274 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_compound_compute | 1 | 0.296 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_logic | 1 | 0.305 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/gather | 1 | 0.293 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/mrg_sort | 2 | 6.891 | make -j |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation | 1 | 0.292 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/select | 1 | 0.307 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/transpose | 1 | 0.301 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum | 2 | 6.696 | make -j |
| 01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tbufpool_management | 1 | 0.286 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/cross_core_set_wait_flag | 2 | 7.010 | make -j |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all | 1 | 1103.900 | python3 ../scripts/verify_result.py output/output.bin output/golden.bin |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/data_movement_with_atomic_operations | 1 | 0.298 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation | 2 | 6.962 | make -j |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_async_iterate | 1 | 1126.684 | python3 ../scripts/verify_result.py output/output.bin output/golden.bin |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8 | 2 | 11.030 | make -j |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareAB | 1 | 0.280 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareB | 1 | 0.291 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/03_libraries/08_tensor_transformation/transdata | 2 | 10.336 | make -j |
| 01_simd_cpp_api/03_libraries/12_math/clampmin | 2 | 10.517 | make -j |
| 01_simd_cpp_api/03_libraries/12_math/power | 2 | 10.299 | make -j |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_basic_api_high_performance | 2 | 6.446 | make -j |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_high_performance | 2 | 11.655 | make -j |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matmul_gelu_high_performance | 2 | 6.764 | make -j |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance | 1 | 0.018 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |
| 01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy | 1 | 0.298 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' '-DCOPY_DST=$COPY_DST' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |

## Slowest 20 Cases

| duration_s | status | case | reason | failing_step |
|---:|---|---|---|---|
| 1126.684 | FAIL | 01_simd_cpp_api/03_libraries/00_matrix/matmul_async_iterate | nonzero rc | python3 ../scripts/verify_result.py output/output.bin output/golden.bin |
| 1121.675 | PASS | 01_simd_cpp_api/03_libraries/00_matrix/matmul_async_iterate_all |  |  |
| 1121.559 | PASS | 01_simd_cpp_api/03_libraries/00_matrix/matmul_callback |  |  |
| 1121.179 | PASS | 01_simd_cpp_api/03_libraries/00_matrix/matmul_channelsplit_output |  |  |
| 1105.253 | PASS | 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/task_sync |  |  |
| 1103.900 | FAIL | 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all | nonzero rc | python3 ../scripts/verify_result.py output/output.bin output/golden.bin |
| 1101.687 | PASS | 01_simd_cpp_api/02_features/04_aicpu/aicpu_device_tiling |  |  |
| 1101.439 | PASS | 01_simd_cpp_api/02_features/03_basic_api/09_scalar_computation/gm_by_pass_dcache |  |  |
| 163.163 | PASS | 01_simd_cpp_api/02_features/02_framework/00_pytorch/torch_library |  |  |
| 150.629 | PASS | 01_simd_cpp_api/01_utilities/04_profiling/torch_library_report_tensor |  |  |
| 150.478 | PASS | 01_simd_cpp_api/02_features/02_framework/00_pytorch/pybind |  |  |
| 85.267 | PASS | 01_simd_cpp_api/03_libraries/13_utils/type_traits |  |  |
| 59.564 | PASS | 01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib |  |  |
| 40.995 | PASS | 01_simd_cpp_api/02_features/05_aclrtc/rtc_template_add |  |  |
| 40.540 | PASS | 01_simd_cpp_api/03_libraries/00_matrix/batch_matmul |  |  |
| 36.795 | PASS | 01_simd_cpp_api/02_features/05_aclrtc/rtc_hello_world |  |  |
| 36.210 | PASS | 01_simd_cpp_api/03_libraries/00_matrix/matmul_sparse |  |  |
| 36.001 | PASS | 01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming |  |  |
| 35.792 | PASS | 01_simd_cpp_api/02_features/00_compilation/custom_op |  |  |
| 35.438 | PASS | 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/create_vec_index |  |  |

## Slowest Passing 15 Cases

| duration_s | case |
|---:|---|
| 1121.675 | 01_simd_cpp_api/03_libraries/00_matrix/matmul_async_iterate_all |
| 1121.559 | 01_simd_cpp_api/03_libraries/00_matrix/matmul_callback |
| 1121.179 | 01_simd_cpp_api/03_libraries/00_matrix/matmul_channelsplit_output |
| 1105.253 | 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/task_sync |
| 1101.687 | 01_simd_cpp_api/02_features/04_aicpu/aicpu_device_tiling |
| 1101.439 | 01_simd_cpp_api/02_features/03_basic_api/09_scalar_computation/gm_by_pass_dcache |
| 163.163 | 01_simd_cpp_api/02_features/02_framework/00_pytorch/torch_library |
| 150.629 | 01_simd_cpp_api/01_utilities/04_profiling/torch_library_report_tensor |
| 150.478 | 01_simd_cpp_api/02_features/02_framework/00_pytorch/pybind |
| 85.267 | 01_simd_cpp_api/03_libraries/13_utils/type_traits |
| 59.564 | 01_simd_cpp_api/02_features/00_compilation/custom_op_static_lib |
| 40.995 | 01_simd_cpp_api/02_features/05_aclrtc/rtc_template_add |
| 40.540 | 01_simd_cpp_api/03_libraries/00_matrix/batch_matmul |
| 36.795 | 01_simd_cpp_api/02_features/05_aclrtc/rtc_hello_world |
| 36.210 | 01_simd_cpp_api/03_libraries/00_matrix/matmul_sparse |

## Artifacts

- `/Users/zhugexun/projects/asc-devkit/presmoke_reports/20260603_jobs4_npu1/REPORT_CN.md`
- `/Users/zhugexun/projects/asc-devkit/presmoke_reports/20260603_jobs4_npu1/FINAL_REPORT.md`
- `/Users/zhugexun/projects/asc-devkit/presmoke_reports/20260603_jobs4_npu1/ALL_CASE_TIMINGS.tsv`
- `/Users/zhugexun/projects/asc-devkit/presmoke_reports/20260603_jobs4_npu1/FAILURES.tsv`
- `/Users/zhugexun/projects/asc-devkit/presmoke_reports/20260603_jobs4_npu1/full_card1/results/report.json`
- `/Users/zhugexun/projects/asc-devkit/presmoke_reports/20260603_jobs4_npu1/full_card1/results/report.md`
- `/Users/zhugexun/projects/asc-devkit/presmoke_reports/20260603_jobs4_npu1/full_card1/meta.txt`
